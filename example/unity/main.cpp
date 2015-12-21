
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

control_t test_repeat(const size_t call_count)
{
    printf("Called for the %u. time\n", call_count);

    return (call_count < 5) ? CaseRepeatHandler : CaseNext;
}

void test_assert_success()
{
    TEST_ASSERT_EQUAL(0, 0);
}

void test_assert_fail()
{
    TEST_ASSERT_EQUAL(0, 1);
}

control_t test_async_fail()
{
    return CaseTimeout(200);
}

void test_async_validate()
{
    Harness::validate_callback();
}

control_t test_async_success()
{
    minar::Scheduler::postCallback(test_async_validate).delay(minar::milliseconds(500)).tolerance(0);
    return CaseTimeout(1000);
}

void test_async_validate_assert_fail()
{
    TEST_ASSERT_NOT_EQUAL(0, 0);
    TEST_ASSERT_EQUAL_PTR(0, 1);
    Harness::validate_callback();
}

control_t test_async_callback_assert_fail()
{
    minar::Scheduler::postCallback(test_async_validate_assert_fail).delay(minar::milliseconds(500)).tolerance(0);
    return CaseTimeout(1000);
}

status_t test_failed_setup(const Case *const source, const size_t index_of_case)
{
    greentea_case_setup_handler(source, index_of_case);
    return STATUS_CONTINUE;
}

status_t test_failed_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    greentea_case_teardown_handler(source, passed, failed, failure);
    return STATUS_CONTINUE;
}

status_t greentea_setup(const size_t number_of_cases)
{
    MBED_HOSTTEST_TIMEOUT(20);
    MBED_HOSTTEST_SELECT(default_auto);
    MBED_HOSTTEST_DESCRIPTION(Basic);
    MBED_HOSTTEST_START("MBED_TEST_GREENTEA_CONTINUE_WITH_UNITY");

    return greentea_test_setup_handler(number_of_cases);
}

const Case cases[] =
{
    Case("NULL test (fail)", ignore_handler, (case_handler_t)ignore_handler, ignore_handler),
    Case("test repeats (success)", test_repeat),
    Case("test assert (fail)", test_failed_setup, test_assert_fail),
    Case("test async (fail)", test_async_fail, test_failed_teardown),
    Case("test async (success)", test_async_success),
    Case("test async callback (fail)", test_async_callback_assert_fail),
    Case("printf with integer formatting (success)", test_assert_success),
};

const Specification specification(greentea_setup, cases, greentea_continue_handlers);


void app_start(int, char*[]) {
    Harness::run(specification);
}
