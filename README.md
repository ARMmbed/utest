# mbed OS Asynchronous Test Harness

This [lest-inspired](https://github.com/martinmoene/lest) test harness allows you to execute a specified series of (asynchronous) C++ test cases with sensible default reporting and useful customization options.

Please note that this is a purposefully lean test harness, only dealing with test execution and providing default reportering handlers. It especially does not support auto-discovery of test cases and does not provide you with test macros or other convenience functions.
Instead the macros in the [unity module](https://github.com/ARMmbed/unity) can be used for this purpose, however, you are not required to and can use your own macros if you want to.

Be aware that even though this harness currently has to be used with C++03, which makes the test case specification a little more annoying, it is written with C++11 in mind.
Especially the use of lambda functions will make the specification a lot more simpler.

Furthermore, test failure recovery via the use of exceptions or `longjmp` is not supported, the test will either continue and ignore failures or die by busy-waiting.

## Theory of Operation

A test specification contains a setup handler, several test cases and a teardown handler.
Each test case contains a textual description, setup, teardown and failure handler as well as the actual test handler.

The order of handler execution is as follows:

1. test setup handler
1. for each test case
    1. test case setup handler
    1. test case execution handler
    1. (wait for timeout or callback validation in case of an asynchronous test case)
    1. (repeat test case execution handler if specified)
    1. test case teardown handler
1. test teardown handler

## Example

The following example showcases a lot of functionality and how to integrate properly with  [Greentea testing automation framework](https://github.com/ARMmbed/greentea) while making use of the [unity test macros](https://github.com/ARMmbed/unity):

```cpp
#include "mbed-drivers/test_env.h"
#include "mbed-test-async/test.h"
#include "unity/unity.h"

using namespace mbed::test::v0;

void test_simple() {
    TEST_ASSERT_EQUAL(0, 0);
    printf("Simple test called\n");
}

status_t test_repeats_setup(const Case *const source, const size_t index_of_case) {
    // call the default handler for proper reporting
    status_t status = verbose_case_set_up_handler(source, index_of_case);
    printf("Setting up for '%s'\n", source->get_description());
    return status;
}
control_flow_t test_repeats(const size_t repeat_count) {
    printf("Called for the %u. time\n", repeat_count+1);
    TEST_ASSERT_NOT_EQUAL(repeat_count, 6);
    // specify how often this test is repeated ie. (n + 1) total calls
    return (repeat_count < 5) ? CONTROL_FLOW_REPEAT : CONTROL_FLOW_NEXT;
}

void test_callback_validate() {
    // you may also use assertions here!
    TEST_ASSERT_EQUAL_PTR(0, 0);
    // validate the callback
    Harness::validate_callback();
}
void test_asynchronous() {
    TEST_ASSERT_MESSAGE(true, "(true == false) o_O");
    // setup a callback in the future, this may also be an interrupt!
    minar::Scheduler::postCallback(test_callback_validate).delay(minar::milliseconds(100));
}

// custom setup handler required for proper Greentea support
status_t greentea_setup(const size_t number_of_cases) {
    MBED_HOSTTEST_TIMEOUT(20);
    MBED_HOSTTEST_SELECT(default_auto);
    MBED_HOSTTEST_DESCRIPTION(Basic);
    MBED_HOSTTEST_START("TEST_EXAMPLE_GREENTEA_SUPPORT");
    // call the default reporting function
    return verbose_test_set_up_handler(number_of_cases);
}

// specify all you test cases here
Case cases[] = {
    Case("Simple Test", test_simple),
    Case("Repeating Test (6x)", test_repeats_setup, test_repeats),
    AsyncCase("Asynchronous Test (200ms timeout)", test_asynchronous, 200)
};

// declare you test specification with a custom setup handler
Specification specification(greentea_setup, cases);

void app_start(int, char*[]) {
    // run the test specification
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
Please see the `mbed-test-async/types.h` file for a detailed description.

1. `status_t test_set_up_handler_t(const size_t number_of_cases)`: is called before execution of any test case
1. `void test_tear_down_handler_t(const size_t passed, const size_t failed, const failure_t failure)`: is called after execution of all test case
1. `status_t case_set_up_handler_t(const Case *const source, const size_t index_of_case)`: is called before execution of each test case
1. `status_t case_tear_down_handler_t(const Case *const source, const size_t passed, const size_t failed, const failure_t reason)`: is called after execution of each test case
1. `status_t case_failure_handler_t(const Case *const source, const failure_t reason)`: is called whenever a failure occurred

All handlers are defaulted for integration with the [Greentea testing automation framework](https://github.com/ARMmbed/greentea).

### Test Case Handlers

There are two test case handlers:

1. `void case_handler_t(void)`: Executes once, if setup succeeded,
1. `control_flow_t case_control_flow_handler_t(const size_t repeat_count)`: Executes as many times as you specify.

Returning `CONTROL_FLOW_REPEAT` from your test case handler tells the test harness to repeat the test handler. You can use the `repeat_count` to decide when to stop. Please note that the setup and teardown handlers will not be called on repeated test cases!

To specify a test case you must wrap it into a `Case` class: `Case("mandatory description", case_handler)`. You may override the setup, teardown and failure handlers in this wrapper class as well.

For asynchronous test cases, you must use the `AsyncCase` class and specify a timeout: `AsyncCase("mandatory description", case_handler, 200)`.
To validate your callback, you must call `Harness::validate_callback()` in your asynchronous callback before the timeout fires.

### Failure Handlers

A failure may occur during any phase of the test. If the setup or teardown handlers fail, they may return a `STATUS_ABORT` code, which will call the failure handler with the appropriate failure reason (`FAILURE_SETUP` and `FAILURE_TEARDOWN` respectively).
If the setup handler fails, the test case is never executed. Instead the teardown handlers is called in an attempt to salvage the situation.
Please note, that if a teardown handler fails, the system can be considered too unstable to continue testing.

You may also raise a failure manually by calling `Harness::raise_failure(failure_t reason)`. In fact, this is how you can integrate assertion failures from custom test macros, as done with the unity macros, which raise a failure with the `FAILURE_ASSERTION` reason.

When waiting for an asynchronous callback and the timeout fired, as `FAILURE_TIMEOUT` is raised.

The failure handler decides whether to continue or abort testing, by returning `STATUS_CONTINUE` or `STATUS_ABORT` respectively.
In case of an abort, the test harness dies by busy waiting in a loop forever.
This is required, since we cannot unwind the stack without exception support, and the asynchronous nature of the test harness breaks with `longjmp`s.

### Default Handlers

Three sets of default handlers with different behaviors are provided for your convenience:

1. `greentea_abort_handlers` (default): greentea-style reporting, abort on the first failure, but requires custom test setup handler
1. `greentea_continue_handlers`: greentea-style reporting, always continues testing, but requires custom test setup handler
1. `verbose_continue_handlers`: always continues testing and reporting, except when a teardown failed

These default handlers are called when you have not overridden a custom handler, and they only contain reporting functionality and do not modify global state.

You can specify which default handlers you want to use when wrapping you test cases in the `Specification` class.

### Custom Handlers

You may override each and every one of the default handler with your own custom handler.

To ignore a handler completely and not call a custom or default handler, you may use the `ignore_handler` hint.
To explicitly invoke the default handler, use the `default_handler` hint.

To use you own custom handler, provide a function with the correct signature for the handler that you want to customize and provide it in your test case wrapper or specification wrapper.
It is strongly recommended that you call the provided `verbose_*` handlers, as they report the current condition in a properly formatted fashion.

Note that the `Case`, `AsyncCase` and `Specification` constructors are overloaded to allow you a comfortable declaration of all your callbacks:

For `Case` and `AsyncCase` the order of arguments is:
1. description (required)
1. setup handler (optional)
1. test case handler (required)
1. teardown handler (optional)
1. failure handler (optional)
1. timeout in ms (required only for `AsyncCase`)

For `Specification` the order of arguments is:
1. test setup handler (optional)
1. array of test cases (required)
1. test teardown handler (optional)
1. default handlers (optional)
