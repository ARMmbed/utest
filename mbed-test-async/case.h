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

    class Case
    {
    public:
        Case(const char *description,
             const case_handler_t case_handler,
             const case_set_up_handler_t set_up_handler = default_handler,
             const case_tear_down_handler_t tear_down_handler = default_handler,
             const case_failure_handler_t failure_handler = default_handler);

        Case(const char *description,
             const case_control_flow_handler_t case_handler,
             const case_set_up_handler_t set_up_handler = default_handler,
             const case_tear_down_handler_t tear_down_handler = default_handler,
             const case_failure_handler_t failure_handler = default_handler);

        const char*
        get_description() const;

        bool
        is_empty() const;

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

        friend class Harness;
    };

    class AsyncCase : public Case
    {
        friend class Harness;
    public:
        AsyncCase(const char *description,
                  const case_handler_t case_handler,
                  const uint32_t timeout_ms,
                  const case_set_up_handler_t set_up_handler = default_handler,
                  const case_tear_down_handler_t tear_down_handler = default_handler,
                  const case_failure_handler_t failure_handler = default_handler);

        AsyncCase(const char *description,
                  const case_control_flow_handler_t case_handler,
                  const uint32_t timeout_ms,
                  const case_set_up_handler_t set_up_handler = default_handler,
                  const case_tear_down_handler_t tear_down_handler = default_handler,
                  const case_failure_handler_t failure_handler = default_handler);
    };
}
}
}

 #endif // MBED_TEST_ASYNC_CASES_H
