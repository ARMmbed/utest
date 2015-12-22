
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

// Validate: Simple Validation ----------------------------------------------------------------------------------------
void simple_validation()
{
    TEST_ASSERT_EQUAL(1, call_counter++);
    Harness::validate_callback();
}
control_t simple_validation_case()
{
    TEST_ASSERT_EQUAL(0, call_counter++);
    minar::Scheduler::postCallback(simple_validation).delay(minar::milliseconds(100));
    return CaseAwait;
}

// Validate: Multiple Validation --------------------------------------------------------------------------------------
void multiple_validation()
{
    // make sure validation is side-effect free
    TEST_ASSERT_EQUAL(3, call_counter++);
    Harness::validate_callback();
    TEST_ASSERT_EQUAL(4, call_counter++);
    Harness::validate_callback();
    TEST_ASSERT_EQUAL(5, call_counter++);
    Harness::validate_callback();
    TEST_ASSERT_EQUAL(6, call_counter++);
    Harness::validate_callback();
    TEST_ASSERT_EQUAL(7, call_counter++);
    Harness::validate_callback();
    TEST_ASSERT_EQUAL(8, call_counter++);
}
control_t multiple_validation_case()
{
    TEST_ASSERT_EQUAL(2, call_counter++);
    minar::Scheduler::postCallback(multiple_validation).delay(minar::milliseconds(100));
    return CaseAwait;
}
status_t multiple_validation_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(1, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(FAILURE_NONE, failure);
    TEST_ASSERT_EQUAL(9, call_counter++);
    return verbose_case_teardown_handler(source, passed, failed, failure);
}

// Validate: Premature Validation -------------------------------------------------------------------------------------
control_t premature_validation_case()
{
    TEST_ASSERT_EQUAL(10, call_counter++);
    /* Prematurely validate the callback.
     * This can happen, when you set up a callback that occurs in an interrupt
     * and it fires and validates the callback before this function completes.
     * The harness only knows whether to expect a callback after the case Handler
     * completes (obviously), so the callback validations are logged.
     */
    Harness::validate_callback();
    return CaseAwait;
}
status_t premature_validation_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(1, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(FAILURE_NONE, failure);
    TEST_ASSERT_EQUAL(11, call_counter++);
    return verbose_case_teardown_handler(source, passed, failed, failure);
}

// Validate: Multiple Premature Validation ----------------------------------------------------------------------------
control_t multiple_premature_validation_case()
{
    TEST_ASSERT_EQUAL(12, call_counter++);
    Harness::validate_callback();
    TEST_ASSERT_EQUAL(13, call_counter++);
    Harness::validate_callback();
    TEST_ASSERT_EQUAL(14, call_counter++);
    Harness::validate_callback();
    TEST_ASSERT_EQUAL(15, call_counter++);
    Harness::validate_callback();
    TEST_ASSERT_EQUAL(16, call_counter++);
    return CaseAwait;
}
status_t multiple_premature_validation_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(1, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(FAILURE_NONE, failure);
    TEST_ASSERT_EQUAL(17, call_counter++);
    return verbose_case_teardown_handler(source, passed, failed, failure);
}

// Validate: Attributed Validation: Cancel Repeat ---------------------------------------------------------------------
void attributed_validation_cancel_repeat()
{
    TEST_ASSERT_EQUAL(19, call_counter++);
    // cancel all repeats
    Harness::validate_callback(CaseNoRepeat);
}
control_t attributed_validation_cancel_repeat_case()
{
    TEST_ASSERT_EQUAL(18, call_counter++);
    minar::Scheduler::postCallback(attributed_validation_cancel_repeat).delay(minar::milliseconds(100));
    // the RepeatAll will be cancelled during callback validation
    return CaseRepeatAll + CaseAwait;
}
status_t attributed_validation_cancel_repeat_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(1, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(FAILURE_NONE, failure);
    TEST_ASSERT_EQUAL(20, call_counter++);
    return verbose_case_teardown_handler(source, passed, failed, failure);
}

// Validate: Attributed Validation: Enable Repeat Handler -------------------------------------------------------------
void attributed_validation_enable_repeat()
{
    TEST_ASSERT_EQUAL(22, call_counter++);
    // cancel all repeats
    Harness::validate_callback(CaseRepeatHandler);
    TEST_ASSERT_EQUAL(23, call_counter++);
    // only the first validation counts
    Harness::validate_callback(CaseNoRepeat);
    TEST_ASSERT_EQUAL(24, call_counter++);
}
control_t attributed_validation_enable_repeat_case(const size_t call_count)
{
    if (call_count == 1) {
        TEST_ASSERT_EQUAL(21, call_counter++);
        minar::Scheduler::postCallback(attributed_validation_enable_repeat).delay(minar::milliseconds(100));
        // the RepeatAll will be cancelled during callback validation
        return CaseAwait;
    }
    TEST_ASSERT_EQUAL(25, call_counter++);
    return CaseNext;
}
status_t attributed_validation_enable_repeat_case_teardown(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(2, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(FAILURE_NONE, failure);
    TEST_ASSERT_EQUAL(26, call_counter++);
    return verbose_case_teardown_handler(source, passed, failed, failure);
}

// Cases --------------------------------------------------------------------------------------------------------------
Case cases[] = {
    Case("Validate: Simple Validation", simple_validation_case),
    Case("Validate: Multiple Validation", multiple_validation_case, multiple_validation_case_teardown),
    Case("Validate: Premature Validation", premature_validation_case, premature_validation_case_teardown),
    Case("Validate: Multiple Premature Validation", multiple_premature_validation_case, multiple_premature_validation_case_teardown),
    Case("Validate: Attributed Validation: Cancel Repeat", attributed_validation_cancel_repeat_case, attributed_validation_cancel_repeat_case_teardown),
    Case("Validate: Attributed Validation: Enable Repeat Handler", attributed_validation_enable_repeat_case, attributed_validation_enable_repeat_case_teardown)
};

status_t greentea_setup(const size_t number_of_cases)
{
    MBED_HOSTTEST_TIMEOUT(15);
    MBED_HOSTTEST_SELECT(default_auto);
    MBED_HOSTTEST_DESCRIPTION(case asynchronous validation);
    MBED_HOSTTEST_START("MBED_OS");

    return verbose_test_setup_handler(number_of_cases);
}
void greentea_teardown(const size_t passed, const size_t failed, const failure_t failure)
{
    TEST_ASSERT_EQUAL(27, call_counter++);
    TEST_ASSERT_EQUAL(6, passed);
    TEST_ASSERT_EQUAL(0, failed);
    TEST_ASSERT_EQUAL(FAILURE_NONE, failure);
    greentea_test_teardown_handler(passed, failed, failure);
}

Specification specification(greentea_setup, cases, greentea_teardown, selftest_handlers);

void app_start(int, char*[])
{
    Harness::run(specification);
}
