# utest: A Simple C++ Test Harness

This [lest-inspired](https://github.com/martinmoene/lest) test harness allows you to execute a specified series of (asynchronous) C++ test cases with sensible default reporting and useful customization options.

Please note that this is a purposefully lean test harness, only dealing with test execution and providing default reporting handlers. It specifically does not support auto-discovery of test cases and does not provide you with test macros or other convenience functions.
Instead, the macros in the [unity module](https://github.com/ARMmbed/unity) can be used for this purpose. However, you are not required to use these, and can use your own macros if you wish.

Be aware that even though this harness currently has to be used with C++03, which makes the test case specification a little more annoying, it is written with C++11 in mind.
In particular, the use of lambda functions will make the specification a lot simpler to write.

Furthermore, test failure recovery through the use of exceptions or `longjmp` is not supported; the test will either continue and ignore failures or die by busy-waiting.

## Theory of Operation

A test specification contains a setup handler, several test cases and a teardown handler.
Each test case contains a textual description, setup, teardown and failure handler as well as the actual test handler.

The order of handler execution is:

1. Test setup handler.
1. For each test case:
    1. Test case setup handler.
    1. Test case execution handler.
    1. (wait for timeout or callback validation in case of an asynchronous test case.)
    1. (repeat test case execution handler if specified.)
    1. Test case teardown handler.
1. Test teardown handler.

## Example

The following example showcases a lot of functionality and proper integration with the [Greentea testing automation framework](https://github.com/ARMmbed/greentea), while making use of the [unity test macros](https://github.com/ARMmbed/unity):

```cpp
#include "mbed-drivers/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"

using namespace utest::v1;

void test_simple() {
    TEST_ASSERT_EQUAL(0, 0);
    printf("Simple test called\n");
}

status_t test_repeats_setup(const Case *const source, const size_t index_of_case) {
    // Call the default handler for proper reporting
    status_t status = verbose_case_setup_handler(source, index_of_case);
    printf("Setting up for '%s'\n", source->get_description());
    return status;
}
control_t test_repeats(const size_t repeat_count) {
    printf("Called for the %u. time\n", repeat_count+1);
    TEST_ASSERT_NOT_EQUAL(repeat_count, 6);
    // Specify how often this test is repeated ie. (n + 1) total calls
    return (repeat_count < 5) ? CaseRepeat : CaseNext;
}

void test_callback_validate() {
    // You may also use assertions here!
    TEST_ASSERT_EQUAL_PTR(0, 0);
    // Validate the callback
    Harness::validate_callback();
}
control_t test_asynchronous() {
    TEST_ASSERT_MESSAGE(true, "(true == false) o_O");
    // Set up a callback in the future. This may also be an interrupt!
    minar::Scheduler::postCallback(test_callback_validate).delay(minar::milliseconds(100));
    // Set a 200ms timeout starting from now
    return CaseTimeout(200);
}

// Custom setup handler required for proper Greentea support
status_t greentea_setup(const size_t number_of_cases) {
    MBED_HOSTTEST_TIMEOUT(20);
    MBED_HOSTTEST_SELECT(default_auto);
    MBED_HOSTTEST_DESCRIPTION(Basic);
    MBED_HOSTTEST_START("TEST_EXAMPLE_GREENTEA_SUPPORT");
    // Call the default reporting function
    return verbose_test_setup_handler(number_of_cases);
}

// Specify all your test cases here
Case cases[] = {
    Case("Simple Test", test_simple),
    Case("Repeating Test (6x)", test_repeats_setup, test_repeats),
    Case("Asynchronous Test (200ms timeout)", test_asynchronous)
};

// Declare your test specification with a custom setup handler
Specification specification(greentea_setup, cases);

void app_start(int, char*[]) {
    // Run the test specification
    Harness::run(specification);
}
```

Running this test will output the following:

```
{{timeout;20}}
{{host_test_name;default_auto}}
{{description;Basic}}
{{test_id;TEST_EXAMPLE_GREENTEA_SUPPORT}}
{{start}}
>>> Running 3 test cases...

>>> Running case #1: 'Simple Test'...
Simple test called
>>> 'Simple Test': 1 passed, 0 failed

>>> Running case #2: 'Repeating Test (6x)'...
Setting up for 'Repeating Test (6x)'
Called for the 1. time
Called for the 2. time
Called for the 3. time
Called for the 4. time
Called for the 5. time
Called for the 6. time
>>> 'Repeating Test (6x)': 6 passed, 0 failed

>>> Running case #3: 'Asynchronous Test (200ms timeout)'...
>>> 'Asynchronous Test (200ms timeout)': 1 passed, 0 failed

>>> Test cases: 3 passed, 0 failed
{{success}}
{{end}}
```

## Detailed Description

### Handlers

There are five handler types you can, but do not have to, override to customize operation.
Please see the `utest/types.h` file for a detailed description.

1. `status_t test_setup_handler_t(const size_t number_of_cases)`: called before execution of any test case.
1. `void test_teardown_handler_t(const size_t passed, const size_t failed, const failure_t failure)`: called after execution of all test cases, and if testing is aborted.
1. `status_t case_setup_handler_t(const Case *const source, const size_t index_of_case)`:called before execution of each test case.
1. `status_t case_teardown_handler_t(const Case *const source, const size_t passed, const size_t failed, const failure_t reason)`: called after execution of each test case, and if testing is aborted.
1. `status_t case_failure_handler_t(const Case *const source, const failure_t reason)`: called whenever a failure occurs.

All handlers are defaulted for integration with the [Greentea testing automation framework](https://github.com/ARMmbed/greentea).

### Test Case Handlers

There are three test case handlers:

1. `void case_handler_t(void)`: executes once, if the case setup succeeded.
1. `control_t case_control_handler_t(void)`: executes (asynchronously) as many times as you specify, if the case setup succeeded.
1. `control_t case_repeat_count_handler_t(const size_t repeat_count)`: executes (asynchronously) as many times as you specify, if the case setup succeeded.

Returning `CaseRepeat` from your test case handler tells the test harness to repeat the test handler. You can use the `repeat_count` (starts counting at zero) to decide when to stop.
By default the setup and teardown handlers are called on every repeated test cases, however, you may only repeat the case handler by returning `CaseRepeatHandlerOnly`. To stop the harness from repeating the test case, return `CaseNext`.

For asynchronous test cases, you must return a `CaseTimeout(uint32_t ms)`.
To validate your callback, you must call `Harness::validate_callback()` in your asynchronous callback before the timeout fires. This will schedule the

For repeating asynchronous cases, you can "add" both modifiers together: `CaseTimeout(200) + CaseRepeat` will repeat the test case after a max. of 200ms.
Note that when adding conflicting modifiers together

- the more restrictive timeout is chosen.
- the more invasive repeat method is chosen: `CaseRepeat` > `CaseRepeatHandlerOnly` > `CaseNext`.

To specify a test case you must wrap it into a `Case` class: `Case("mandatory description", case_handler)`. You may override the setup, teardown and failure handlers in this wrapper class as well.

Keep in mind that you can only validate a callback once. If you need to wait for several callbacks, you need to write your own helper function that validates the expected callback only when all your custom callbacks arrive.
This custom functionality is purposefully not part of this test harness, you can achieve it externally with additional code.

### Failure Handlers

A failure may occur during any phase of the test. If the setup or teardown handlers fail, they may return a `STATUS_ABORT` code, which will call the failure handler with the appropriate failure reason (`FAILURE_SETUP` and `FAILURE_TEARDOWN` respectively).
If the setup handler fails, the test case is never executed. Instead, the teardown handler is called in an attempt to salvage the situation.
Please note that if a teardown handler fails, the system can be considered too unstable to continue testing.

You may also raise a failure manually by calling `Harness::raise_failure(failure_t reason)`. In fact, this is how you can integrate assertion failures from custom test macros, as done with the unity macros, which raise a failure with the `FAILURE_ASSERTION` reason.

When waiting for an asynchronous callback, if the timeout fires,  `FAILURE_TIMEOUT` is raised.

The failure handler decides whether to continue or abort testing by returning `STATUS_CONTINUE` or `STATUS_ABORT` respectively.
In case of an abort, the test harness dies by busy waiting in a forever loop.
This is needed because we cannot unwind the stack without exception support, and the asynchronous nature of the test harness breaks with using `longjmp`s.

### Default Handlers

Three sets of default handlers with different behaviors are provided for your convenience:

1. `greentea_abort_handlers` (default): Greentea-style reporting, aborts on the first failure, but requires custom test setup handler.
1. `greentea_continue_handlers`: Greentea-style reporting, always continues testing, but requires custom test setup handler.
1. `verbose_continue_handlers`: always continues testing and reporting, except when a teardown failed.

These default handlers are called when you have not overridden a custom handler, and they only contain reporting functionality and do not modify global state.

You can specify which default handlers you want to use when wrapping your test cases in the `Specification` class:

```cpp
// Declare your test specification with a custom setup handler
// and set the default handlers to the predefined “greentea continue” behavior
Specification specification(greentea_setup, cases, greentea_continue_handlers);
```

### Custom Handlers

You may override any of the default handlers with your own custom handler.

To ignore a handler completely and not call a custom or default handler, you may use the `ignore_handler` hint.
To explicitly invoke the default handler, use the `default_handler` hint.

To use your own custom handler, provide a function with the correct signature for the handler that you want to customize and provide it in your test case wrapper or specification wrapper.
To turn a `failure_t` into a meaningful string use the `stringify(failure_t)` method.

**We strongly recommend that you call the predefined `verbose_*` handlers inside your custom callback, as they report the current condition in a properly formatted fashion.**

Note that the `Case` and `Specification` constructors are overloaded to allow you a comfortable declaration of all your callbacks.

For `Case` the order of arguments is:

1. Description (required).
1. Setup handler (optional).
1. Test case handler (required).
1. Teardown handler (optional).
1. Failure handler (optional).

For `Specification` the order of arguments is:

1. Test setup handler (optional).
1. Array of test cases (required).
1. Test teardown handler (optional).
1. Default handlers (optional).

### Atomicity

All handlers execute with interrupts enabled, **except the case failure handler!**.
This means you can write test cases that poll for interrupts to be completed inside any handler, except the failure handler.

If you setup an interrupt that validates its callback using `Harness::validate_callback()` inside a test case and it fires before the test case completed, the validation will be buffered.
If the test case then returns a timeout value, but the callback is already validated, the test harness just continues normally.
