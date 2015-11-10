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

    enum control_flow_t {
        CONTROL_FLOW_NEXT   = 0,
        CONTROL_FLOW_REPEAT = 1,
    };

    enum status_t {
        STATUS_CONTINUE = 0,
        STATUS_ABORT,
    };

    enum failure_t {
        FAILURE_NONE = 0,
        FAILURE,
        FAILURE_SETUP,
        FAILURE_TEARDOWN,
        FAILURE_TIMEOUT,
        FAILURE_ASSERTION,
    };

    class Case; // forward declaration

    typedef status_t (*test_set_up_handler_t)   (const size_t number_of_cases);
    typedef void     (*test_tear_down_handler_t)(const size_t passed, const size_t failed, const failure_t failure);

    typedef void           (*case_handler_t)             (void);
    typedef control_flow_t (*case_control_flow_handler_t)(void);

    typedef status_t (*case_set_up_handler_t)   (const Case *const source, const size_t index_of_test);
    typedef status_t (*case_tear_down_handler_t)(const Case *const source, const size_t passed, const size_t failed);
    typedef status_t (*case_failure_handler_t)(const Case *const source, const failure_t reason);


    status_t default_case_set_up_handler   (const Case *const source, const size_t index_of_case);
    status_t default_case_tear_down_handler(const Case *const source, const size_t passed, const size_t failed);
    status_t default_case_failure_handler  (const Case *const source, const failure_t reason);

    status_t default_test_set_up_handler   (const size_t number_of_cases);
    void     default_test_tear_down_handler(const size_t passed, const size_t failed, const failure_t failure);


    class Case
    {
    public:
        Case(const char *description,
             const case_handler_t case_handler,
             const case_set_up_handler_t set_up_handler = default_case_set_up_handler,
             const case_tear_down_handler_t tear_down_handler = default_case_tear_down_handler,
             const case_failure_handler_t failure_handler = default_case_failure_handler);

        Case(const char *description,
             const case_control_flow_handler_t case_handler,
             const case_set_up_handler_t set_up_handler = default_case_set_up_handler,
             const case_tear_down_handler_t tear_down_handler = default_case_tear_down_handler,
             const case_failure_handler_t failure_handler = default_case_failure_handler);

        const char*
        get_description() const;

    protected:
        Case(const char *description,
             const case_handler_t case_handler,
             const case_control_flow_handler_t control_flow_handler,
             const case_set_up_handler_t set_up_handler,
             const case_tear_down_handler_t tear_down_handler,
             const case_failure_handler_t failure_handler,
             const int32_t timeout_ms);

    private:
        const char *description;

        const case_handler_t handler;
        const case_control_flow_handler_t control_flow_handler;

        const case_set_up_handler_t set_up_handler;
        const case_tear_down_handler_t tear_down_handler;

        const case_failure_handler_t failure_handler;

        const int32_t timeout_ms;

        friend class TestHarness;
    };

    class AsyncCase : public Case
    {
        friend class TestHarness;
    public:
        AsyncCase(const char *description,
                  const case_handler_t case_handler,
                  const uint32_t timeout_ms,
                  const case_set_up_handler_t set_up_handler = default_case_set_up_handler,
                  const case_tear_down_handler_t tear_down_handler = default_case_tear_down_handler,
                  const case_failure_handler_t failure_handler = default_case_failure_handler);

        AsyncCase(const char *description,
                  const case_control_flow_handler_t case_handler,
                  const uint32_t timeout_ms,
                  const case_set_up_handler_t set_up_handler = default_case_set_up_handler,
                  const case_tear_down_handler_t tear_down_handler = default_case_tear_down_handler,
                  const case_failure_handler_t failure_handler = default_case_failure_handler);
    };

    typedef Case Test;

    class TestHarness
    {
    public:
        template< size_t N >
        static void run(Test (&specification)[N],
                        const test_set_up_handler_t set_up_handler = default_test_set_up_handler,
                        const test_tear_down_handler_t tear_down_handler = default_test_tear_down_handler) {
            run(specification, N, set_up_handler, tear_down_handler);
        }

        static void validate_callback();

        static void raise_failure(failure_t reason);

    protected:
        static void run(const Test *const specification,
                        const size_t length,
                        const test_set_up_handler_t set_up_handler,
                        const test_tear_down_handler_t tear_down_handler);

        static void run_next_case();
        static void handle_timeout();
        static void schedule_next_case();
    };

}
}
}

 #endif // MBED_ASYNC_TEST_H
