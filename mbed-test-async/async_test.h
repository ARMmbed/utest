/****************************************************************************
 * Copyright (c) 2015, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ****************************************************************************
 */

 #ifndef MBED_ASYNC_TEST_H
 #define MBED_ASYNC_TEST_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "unity/unity.h"


namespace mbed {
namespace test {
namespace v0 {

    // the test
    enum status_t {
        STATUS_SUCCESS  = 0,
        STATUS_CONTINUE = 0,

        STATUS_ABORT    = 1,
        STATUS_FAILURE = 0x2000,
        STATUS_FAILURE_TIMEOUT,
        STATUS_FAILURE_ASSERTION,

        STATUS_EXPECT_ASYNC_CALL = 0x4000,
    };

    enum event_t {
        EVENT_NONE = 0,
        EVENT_ASYNC_CALL,
    };

    // forward declaration
    class Case;

    typedef status_t (*case_handler_t)(void);
    typedef status_t (*case_failure_handler_t)(const Case *const source, const status_t reason);

    class Case
    {
    public:
        Case(const char *description, const case_handler_t case_handler);

        Case(const char *description, const case_handler_t case_handler, const uint32_t repeats);

        Case(const char *description, const case_handler_t case_handler, const case_failure_handler_t failure_handler);

        Case(const char *description, const case_handler_t case_handler, const uint32_t repeats, const case_failure_handler_t failure_handler);

        const char*
        getDescription() const;

    protected:
        Case(const char *description, const case_handler_t case_handler, const uint32_t repeats, const case_failure_handler_t failure_handler, const uint32_t timeout_ms);

        const char *description;

        const case_handler_t case_handler;
        const uint32_t repeats;
        const case_failure_handler_t failure_handler;

        const int32_t timeout_ms;
        // const uint32_t *const expected_calls;

        friend class TestHarness;
    };

    class AsyncCase : public Case
    {
        friend class TestHarness;
    public:
        AsyncCase(const char *description, const case_handler_t case_handler, const uint32_t timeout_ms);

        AsyncCase(const char *description, const case_handler_t case_handler, const uint32_t repeats, const uint32_t timeout_ms);

        AsyncCase(const char *description, const case_handler_t case_handler, const case_failure_handler_t failure_handler, const uint32_t timeout_ms);

        AsyncCase(const char *description, const case_handler_t case_handler, const uint32_t repeats, const case_failure_handler_t failure_handler, const uint32_t timeout_ms);
    };

    typedef Case Test;
    typedef status_t (*set_up_handler_t)(const size_t number_of_tests);
    typedef void (*tear_down_handler_t)(const size_t passed, const size_t failed);

    status_t default_failure_handler(const Case *const source, const status_t reason);
    status_t default_set_up_handler(const size_t number_of_tests);
    void default_tear_down_handler(const size_t passed, const size_t failed);

    class TestHarness
    {
    public:
        template< size_t N >
        static void run(Test (&specification)[N], const set_up_handler_t set_up = default_set_up_handler, const tear_down_handler_t tear_down = default_tear_down_handler) {
            run(specification, N, set_up, tear_down);
        }

        static void validateCallback();

    protected:
        static void run(const Test *const specification, const size_t length, const set_up_handler_t set_up, const tear_down_handler_t tear_down);
        static void run_next_test();
        static void handle_failure(status_t reason);
        static void handle_timeout();
    };

}
}
}

 #endif // MBED_ASYNC_TEST_H
