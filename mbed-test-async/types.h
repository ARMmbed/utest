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

#ifndef MBED_TEST_ASYNC_TYPES_H
#define MBED_TEST_ASYNC_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>


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
        FAILURE_CASES,
    };

    class Case; // forward declaration

    typedef status_t (*test_set_up_handler_t)   (const size_t number_of_cases);
    typedef void     (*test_tear_down_handler_t)(const size_t passed, const size_t failed, const failure_t failure);

    typedef void           (*case_handler_t)             (void);
    typedef control_flow_t (*case_control_flow_handler_t)(void);

    typedef status_t (*case_set_up_handler_t)   (const Case *const source, const size_t index_of_test);
    typedef status_t (*case_tear_down_handler_t)(const Case *const source, const size_t passed, const size_t failed, const failure_t reason);
    typedef status_t (*case_failure_handler_t)(const Case *const source, const failure_t reason);

}
}
}

#endif // MBED_TEST_ASYNC_TYPES_H
