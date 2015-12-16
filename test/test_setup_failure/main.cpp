
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

void test_dummy() { /* tumbleweed */ }
Case cases[] =
{
    Case("dummy test", test_dummy),
    Case("dummy test 2", test_dummy)
};

// this setup handler fails
status_t failing_setup_handler(const size_t number_of_cases)
{
    MBED_HOSTTEST_TIMEOUT(5);
    MBED_HOSTTEST_SELECT(default_auto);
    MBED_HOSTTEST_DESCRIPTION(test setup failure test);
    MBED_HOSTTEST_START("MBED_A1");

    TEST_ASSERT_EQUAL(2, number_of_cases);
    greentea_test_setup_handler(number_of_cases);
    return STATUS_ABORT;    // aborting test
};
// the teardown handler will then be called with the reason `FAILURE_SETUP`
void failing_teardown_handler(const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(0, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(FAILURE_SETUP, failure);

    verbose_test_teardown_handler(passed, failed, failure);

    if (failure == FAILURE_SETUP) MBED_HOSTTEST_RESULT(true);
};

Specification specification(failing_setup_handler, cases, failing_teardown_handler);

void app_start(int, char*[])
{
    Harness::run(specification);
}
