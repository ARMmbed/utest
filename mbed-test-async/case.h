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

#ifndef MBED_TEST_ASYNC_CASES_H
#define MBED_TEST_ASYNC_CASES_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "types.h"
#include "default_handlers.h"

namespace mbed {
namespace test {
namespace v0 {

    /** @brief Test case wrapper class.
     *
     * This class contains the description of the test case and all handlers
     * for setting up, running the test case, tearing down and handling failures.
     *
     * By default you only need to provide a description and a test case handler.
     * You may override the setup, teardown and failure handlers, but you do not have to.
     * If you do not override these handler, the specified default handlers will be called.
     *
     * These constructors are overloaded to allow you a comfortable declaration of all your
     * callbacks.
     * The order is always:
     *  - description (required)
     *  - setup handler (optional)
     *  - test case handler (required)
     *  - teardown handler (optional)
     *  - failure handler (optional)
     *
     * @note While you can specify an empty test case (ie. use `ignore_handler` for all callbacks),
     *       the harness will abort the test unconditionally.
     */
    class Case
    {
    public:
        Case(const char *description,
             const case_setup_handler_t setup_handler,
             const case_handler_t case_handler,
             const case_teardown_handler_t teardown_handler = default_handler,
             const case_failure_handler_t failure_handler = default_handler);

        Case(const char *description,
             const case_handler_t case_handler,
             const case_failure_handler_t failure_handler = default_handler);

        Case(const char *description,
             const case_handler_t case_handler,
             const case_teardown_handler_t teardown_handler,
             const case_failure_handler_t failure_handler = default_handler);


        Case(const char *description,
            const case_setup_handler_t setup_handler,
            const case_control_flow_handler_t case_handler,
            const case_teardown_handler_t teardown_handler = default_handler,
            const case_failure_handler_t failure_handler = default_handler);

        Case(const char *description,
            const case_control_flow_handler_t case_handler,
            const case_failure_handler_t failure_handler = default_handler);

        Case(const char *description,
            const case_control_flow_handler_t case_handler,
            const case_teardown_handler_t teardown_handler,
            const case_failure_handler_t failure_handler = default_handler);


        const char*
        get_description() const;

        bool
        is_empty() const;

    protected:
        Case(const char *description,
             const case_setup_handler_t setup_handler,
             const case_handler_t case_handler,
             const case_control_flow_handler_t control_flow_handler,
             const case_teardown_handler_t teardown_handler,
             const case_failure_handler_t failure_handler,
             const int32_t timeout_ms);

    private:
        const char *description;

        const case_handler_t handler;
        const case_control_flow_handler_t control_flow_handler;

        const case_setup_handler_t setup_handler;
        const case_teardown_handler_t teardown_handler;

        const case_failure_handler_t failure_handler;

        const int32_t timeout_ms;

        friend class Harness;
    };

    /** @brief Test case wrapper class for asynchronous calls.
     *
     * This class only expands on the base class by including a timeout (in milliseconds)
     * after which the test is declared as failed.
     * You need to call `Harness::validate_callback();` to cancel the timeout.
     *
     * These constructors are overloaded to allow you a comfortable declaration of all your
     * callbacks.
     * The order is always:
     *  - description (required)
     *  - setup handler (optional)
     *  - test case handler (required)
     *  - teardown handler (optional)
     *  - failure handler (optional)
     *  - timeout in ms (required)
     */
    class AsyncCase : public Case
    {
        friend class Harness;

    public:
        AsyncCase(const char *description,
                  const case_handler_t case_handler,
                  const uint32_t timeout_ms);

        AsyncCase(const char *description,
                  const case_setup_handler_t setup_handler,
                  const case_handler_t case_handler,
                  const uint32_t timeout_ms);


        AsyncCase(const char *description,
                  const case_handler_t case_handler,
                  const case_teardown_handler_t teardown_handler,
                  const uint32_t timeout_ms);

        AsyncCase(const char *description,
                  const case_setup_handler_t setup_handler,
                  const case_handler_t case_handler,
                  const case_teardown_handler_t teardown_handler,
                  const uint32_t timeout_ms);


        AsyncCase(const char *description,
                  const case_handler_t case_handler,
                  const case_failure_handler_t failure_handler,
                  const uint32_t timeout_ms);

        AsyncCase(const char *description,
                  const case_setup_handler_t setup_handler,
                  const case_handler_t case_handler,
                  const case_failure_handler_t failure_handler,
                  const uint32_t timeout_ms);


        AsyncCase(const char *description,
                  const case_handler_t case_handler,
                  const case_teardown_handler_t teardown_handler,
                  const case_failure_handler_t failure_handler,
                  const uint32_t timeout_ms);

        AsyncCase(const char *description,
                  const case_setup_handler_t setup_handler,
                  const case_handler_t case_handler,
                  const case_teardown_handler_t teardown_handler,
                  const case_failure_handler_t failure_handler,
                  const uint32_t timeout_ms);

    public:
        AsyncCase(const char *description,
                  const case_control_flow_handler_t case_handler,
                  const uint32_t timeout_ms);

        AsyncCase(const char *description,
                  const case_setup_handler_t setup_handler,
                  const case_control_flow_handler_t case_handler,
                  const uint32_t timeout_ms);


        AsyncCase(const char *description,
                  const case_control_flow_handler_t case_handler,
                  const case_teardown_handler_t teardown_handler,
                  const uint32_t timeout_ms);

        AsyncCase(const char *description,
                  const case_setup_handler_t setup_handler,
                  const case_control_flow_handler_t case_handler,
                  const case_teardown_handler_t teardown_handler,
                  const uint32_t timeout_ms);


        AsyncCase(const char *description,
                  const case_control_flow_handler_t case_handler,
                  const case_failure_handler_t failure_handler,
                  const uint32_t timeout_ms);

        AsyncCase(const char *description,
                  const case_setup_handler_t setup_handler,
                  const case_control_flow_handler_t case_handler,
                  const case_failure_handler_t failure_handler,
                  const uint32_t timeout_ms);


        AsyncCase(const char *description,
                  const case_control_flow_handler_t case_handler,
                  const case_teardown_handler_t teardown_handler,
                  const case_failure_handler_t failure_handler,
                  const uint32_t timeout_ms);

        AsyncCase(const char *description,
                  const case_setup_handler_t setup_handler,
                  const case_control_flow_handler_t case_handler,
                  const case_teardown_handler_t teardown_handler,
                  const case_failure_handler_t failure_handler,
                  const uint32_t timeout_ms);
    };
}
}
}

 #endif // MBED_TEST_ASYNC_CASES_H
