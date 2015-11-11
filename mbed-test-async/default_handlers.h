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

#ifndef MBED_TEST_ASYNC_DEFAULT_HANDLER_H
#define MBED_TEST_ASYNC_DEFAULT_HANDLER_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "types.h"


namespace mbed {
namespace test {
namespace v0 {

    const struct
    {
        operator test_set_up_handler_t()    const { return test_set_up_handler_t(1); }
        operator test_tear_down_handler_t() const { return test_tear_down_handler_t(1); }

        operator case_set_up_handler_t()    const { return case_set_up_handler_t(1); }
        operator case_tear_down_handler_t() const { return case_tear_down_handler_t(1); }
        operator case_failure_handler_t()   const { return case_failure_handler_t(1); }
    } default_handler;

    struct handlers_t
    {
        test_set_up_handler_t test_set_up;
        test_tear_down_handler_t test_tear_down;

        case_set_up_handler_t case_set_up;
        case_tear_down_handler_t case_tear_down;
        case_failure_handler_t case_failure;

        inline test_set_up_handler_t get_handler(test_set_up_handler_t handler) const {
            if (handler == default_handler) return test_set_up;
            return handler;
        }
        inline test_tear_down_handler_t get_handler(test_tear_down_handler_t handler) const {
            if (handler == default_handler) return test_tear_down;
            return handler;
        }

        inline case_set_up_handler_t get_handler(case_set_up_handler_t handler) const {
            if (handler == default_handler) return case_set_up;
            return handler;
        }
        inline case_tear_down_handler_t get_handler(case_tear_down_handler_t handler) const {
            if (handler == default_handler) return case_tear_down;
            return handler;
        }
        inline case_failure_handler_t get_handler(case_failure_handler_t handler) const {
            if (handler == default_handler) return case_failure;
            return handler;
        }
    };

    status_t verbose_test_set_up_handler   (const size_t number_of_cases);
    void     verbose_test_tear_down_handler(const size_t passed, const size_t failed, const failure_t failure);

    status_t verbose_case_set_up_handler   (const Case *const source, const size_t index_of_case);
    status_t verbose_case_tear_down_handler(const Case *const source, const size_t passed, const size_t failed);
    status_t verbose_case_failure_handler  (const Case *const source, const failure_t reason);

    const handlers_t verbose_handlers = {
        verbose_test_set_up_handler,
        verbose_test_tear_down_handler,
        verbose_case_set_up_handler,
        verbose_case_tear_down_handler,
        verbose_case_failure_handler
    };


    status_t greentea_test_set_up_handler   (const size_t number_of_cases);
    void     greentea_test_tear_down_handler(const size_t passed, const size_t failed, const failure_t failure);

    status_t greentea_case_failure_handler  (const Case *const source, const failure_t reason);

    const handlers_t greentea_handlers = {
        greentea_test_set_up_handler,
        greentea_test_tear_down_handler,
        verbose_case_set_up_handler,
        verbose_case_tear_down_handler,
        greentea_case_failure_handler
    };
}
}
}

#endif // MBED_TEST_ASYNC_DEFAULT_HANDLER_H
