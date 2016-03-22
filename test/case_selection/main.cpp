
/* mbed Microcontroller Library
 * Copyright (c) 2013-2016 ARM Limited
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
#include "greentea-client/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"


using namespace utest::v1;

int call_counter(0);

void handler_case_2()
{
    printf("Executing Case 2...\n");
}
status_t teardown_case_2(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(1, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(REASON_NONE, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_NONE, failure.location);
    TEST_ASSERT_EQUAL(0, call_counter++);
    greentea_case_teardown_handler(source, passed, failed, failure);
    return status_t(0);
}
void handler_case_0()
{
    printf("Executing Case 0...\n");
}
status_t teardown_case_0(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(1, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(REASON_NONE, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_NONE, failure.location);
    TEST_ASSERT_EQUAL(1, call_counter++);
    greentea_case_teardown_handler(source, passed, failed, failure);
    return status_t(1);
}
void handler_case_1()
{
    printf("Executing Case 1...\n");
}
status_t teardown_case_1(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(1, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(REASON_NONE, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_NONE, failure.location);
    TEST_ASSERT_EQUAL(2, call_counter++);
    greentea_case_teardown_handler(source, passed, failed, failure);
    return status_t(3);
}

Case cases[] =
{
    Case("Case 1", handler_case_0, teardown_case_0),
    Case("Case 2", handler_case_1, teardown_case_1),
    Case("Case 3", handler_case_2, teardown_case_2)
};

status_t test_setup_handler(const size_t number_of_cases)
{
    GREENTEA_SETUP(5, "default_auto");

    greentea_test_setup_handler(number_of_cases);
    return status_t(2);
};
void test_teardown_handler(const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(3, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(REASON_NONE, failure.reason);
    TEST_ASSERT_EQUAL(LOCATION_NONE, failure.location);
    TEST_ASSERT_EQUAL(3, call_counter++);

    greentea_test_teardown_handler(passed, failed, failure);
};

Specification specification(test_setup_handler, cases, test_teardown_handler, selftest_handlers);

void app_start(int, char*[])
{
    Harness::run(specification);
}
