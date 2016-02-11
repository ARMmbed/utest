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
#include "mbed-drivers/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"

using namespace utest::v1;

// Specify all your test cases here
Case cases[] =
{
    Case("Simple Test", []() {
        TEST_ASSERT_EQUAL(0, 0);
        printf("Simple test called\n");
    }),
    Case("Repeating Test", [](const Case *const source, const size_t index_of_case) {
        // Call the default handler for proper reporting
        status_t status = greentea_case_setup_handler(source, index_of_case);
        printf("Setting up for '%s'\n", source->get_description());
        return status;
    },
    [](const size_t call_count) {
        printf("Called for the %u. time\n", call_count);
        TEST_ASSERT_NOT_EQUAL(3, call_count);
        // Specify how often this test is repeated ie. n total calls
        return (call_count < 2) ? CaseRepeatAll : CaseNext;
    }),
    Case("Asynchronous Test (200ms timeout)", []() -> control_t { // optional return type declaration
        TEST_ASSERT_TRUE_MESSAGE(true, "(true == false) o_O");
        // Set up a callback in the future. This may also be an interrupt!
        minar::Scheduler::postCallback([]()
        {   // You may also use assertions here!
            TEST_ASSERT_EQUAL_PTR(0, 0);
            // Validate the callback
            Harness::validate_callback();
        }).delay(minar::milliseconds(100));
        // Set a 200ms timeout starting from now
        return CaseTimeout(200);
    }),
    Case("Asynchronous Timeout Repeat", [](const size_t call_count) {
        TEST_ASSERT_TRUE_MESSAGE(true, "(true == false) o_O");
        // Set a 200ms timeout starting from now,
        // but automatically repeat only this handler on timeout.
        if (call_count >= 5) {
            // but after the 5th retry, the callback finally gets validated
            minar::Scheduler::postCallback([]()
            {   // Validate the callback
                Harness::validate_callback();
            }).delay(minar::milliseconds(100));
        }
        return CaseRepeatHandlerOnTimeout(200);
    })
};

 // Declare your test specification with a custom setup handler
Specification specification([](const size_t number_of_cases) {
    GREENTEA_SETUP(20, "default_auto");

    // Call the default reporting function
    return greentea_test_setup_handler(number_of_cases);
}, cases);

void app_start(int, char*[]) {
    Harness::run(specification);
}
