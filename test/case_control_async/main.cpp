
/* mbed Microcontroller Library
 * Copyright (c) 2013-2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "mbed-drivers/mbed.h"
#include "mbed-drivers/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"

using namespace utest::v1;

int call_counter(0);

// Control: Timeout (Failure) -----------------------------------------------------------------------------------------
control_t timeout_failure_case(const size_t call_count)
{
    TEST_ASSERT_EQUAL(1, call_count);
    TEST_ASSERT_EQUAL(0, call_counter++);
    return CaseTimeout(100);
}
status_t timeout_failure_case_failure_handler(const Case *const source, const failure_t reason)
{
    TEST_ASSERT_EQUAL(1, call_counter++);
    TEST_ASSERT_EQUAL(FAILURE_TIMEOUT, reason);
    verbose_case_failure_handler(source, reason);
    return STATUS_CONTINUE;
}
status_t timeout_failure_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(2, call_counter++);
    TEST_ASSERT_EQUAL(0, passed);
    TEST_ASSERT_EQUAL(1, failed);
    TEST_ASSERT_EQUAL(FAILURE_CASES, failure);
    return greentea_case_teardown_handler(source, passed, failed, failure);
}

// Control: Timeout (Success) -----------------------------------------------------------------------------------------
void timeout_success_case_validate() {
    TEST_ASSERT_EQUAL(4, call_counter++);
    Harness::validate_callback();
}
control_t timeout_success_case(const size_t call_count)
{
    TEST_ASSERT_EQUAL(1, call_count);
    TEST_ASSERT_EQUAL(3, call_counter++);
    minar::Scheduler::postCallback(timeout_success_case_validate).delay(minar::milliseconds(100));
    return CaseTimeout(200);
}
status_t timeout_success_case_failure_handler(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(5, call_counter++);
    TEST_ASSERT_EQUAL(1, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(FAILURE_NONE, failure);
    return greentea_case_teardown_handler(source, passed, failed, failure);
}

// Control: Await -----------------------------------------------------------------------------------------------------
void await_case_validate(int expected_call_count) {
    TEST_ASSERT_EQUAL(expected_call_count, call_counter++);
    Harness::validate_callback();
}
control_t await_case(const size_t call_count)
{
    TEST_ASSERT_EQUAL(1, call_count);
    TEST_ASSERT_EQUAL(6, call_counter++);
    minar::Scheduler::postCallback(mbed::util::FunctionPointer1<void, int>(await_case_validate).bind(7)).delay(minar::milliseconds(1372));
    return CaseAwait;
}

// Control: RepeatAllOnTimeout ----------------------------------------------------------------------------------------
status_t repeat_all_on_timeout_case_setup(const Case *const source, const size_t index_of_case)
{
    static int repeat_counter(0);
    TEST_ASSERT_EQUAL(3, index_of_case);
    TEST_ASSERT_EQUAL(repeat_counter*3 + 8, call_counter++);
    repeat_counter++;
    return greentea_case_setup_handler(source, index_of_case);
}
control_t repeat_all_on_timeout_case(const size_t call_count)
{
    printf("Running case handler for %u. time\n", call_count);
    static int repeat_counter(1);
    TEST_ASSERT_EQUAL(repeat_counter++, call_count);
    TEST_ASSERT(call_count <= 10);
    TEST_ASSERT_EQUAL((call_count-1)*3 + 9, call_counter++);
    if (call_count == 10) {
        minar::Scheduler::postCallback(mbed::util::FunctionPointer1<void, int>(await_case_validate).bind(37)).delay(minar::milliseconds(50));
    }
    return CaseRepeatAllOnTimeout(100);
}
status_t repeat_all_on_timeout_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    static int repeat_counter(0);
    TEST_ASSERT_EQUAL((call_counter == 38) ? 1 : 0, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(FAILURE_NONE, failure);
    TEST_ASSERT_EQUAL(repeat_counter*3 + ((repeat_counter == 9) ? 11 : 10), call_counter++);
    repeat_counter++;
    return greentea_case_teardown_handler(source, passed, failed, failure);
}

// Control: RepeatAllOnTimeout ----------------------------------------------------------------------------------------
status_t repeat_handler_on_timeout_case_setup(const Case *const source, const size_t index_of_case)
{
    TEST_ASSERT_EQUAL(4, index_of_case);
    TEST_ASSERT_EQUAL(39, call_counter++);
    return greentea_case_setup_handler(source, index_of_case);
}
control_t repeat_handler_on_timeout_case(const size_t call_count)
{
    printf("Running case handler for %u. time\n", call_count);
    static int repeat_counter(1);
    TEST_ASSERT_EQUAL(repeat_counter++, call_count);
    TEST_ASSERT(call_count <= 10);
    TEST_ASSERT_EQUAL(call_count-1 + 40, call_counter++);
    if (call_count == 10) {
        minar::Scheduler::postCallback(mbed::util::FunctionPointer1<void, int>(await_case_validate).bind(50)).delay(minar::milliseconds(50));
    }
    return CaseRepeatHandlerOnTimeout(100);
}
status_t repeat_handler_on_timeout_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(1, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(FAILURE_NONE, failure);
    TEST_ASSERT_EQUAL(51, call_counter++);
    return greentea_case_teardown_handler(source, passed, failed, failure);
}

// Control: NoTimeout -------------------------------------------------------------------------------------------------
control_t no_timeout_case(const size_t call_count)
{
    TEST_ASSERT_EQUAL(1, call_count);
    TEST_ASSERT_EQUAL(52, call_counter++);
    return CaseNoTimeout;
}

// Control: NoTimeout -------------------------------------------------------------------------------------------------
control_t next_case(const size_t call_count)
{
    TEST_ASSERT_EQUAL(1, call_count);
    TEST_ASSERT_EQUAL(53, call_counter++);
    return CaseNoTimeout;
}

// Cases --------------------------------------------------------------------------------------------------------------
Case cases[] = {
    Case("Control: Timeout (Failure)", timeout_failure_case, timeout_failure_case_teardown, timeout_failure_case_failure_handler),
    Case("Control: Timeout (Success)", timeout_success_case, timeout_success_case_failure_handler),
    Case("Control: Await", await_case),
    Case("Control: RepeatAllOnTimeout", repeat_all_on_timeout_case_setup, repeat_all_on_timeout_case, repeat_all_on_timeout_case_teardown),
    Case("Control: RepeatHandlerOnTimeout", repeat_handler_on_timeout_case_setup, repeat_handler_on_timeout_case, repeat_handler_on_timeout_case_teardown),
    Case("Control: NoTimeout", no_timeout_case),
    Case("Control: CaseNext", next_case)
};

// Specification: Setup & Teardown ------------------------------------------------------------------------------------
status_t greentea_setup(const size_t number_of_cases)
{
    MBED_HOSTTEST_TIMEOUT(15);
    MBED_HOSTTEST_SELECT(default_auto);
    MBED_HOSTTEST_DESCRIPTION(case control handler test);
    MBED_HOSTTEST_START("MBED_OS");

    return verbose_test_setup_handler(number_of_cases);
}
void greentea_teardown(const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(54, call_counter++);
    TEST_ASSERT_EQUAL(6, passed);
    TEST_ASSERT_EQUAL(1, failed);
    TEST_ASSERT_EQUAL(FAILURE_CASES, failure);
    verbose_test_teardown_handler(passed, failed, failure);
    MBED_HOSTTEST_RESULT(true);
}

Specification specification(greentea_setup, cases, greentea_teardown, selftest_handlers);

void app_start(int, char*[])
{
    Harness::run(specification);
}
