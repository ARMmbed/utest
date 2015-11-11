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

    class Case;

    status_t default_test_set_up_handler   (const size_t number_of_cases);
    void     default_test_tear_down_handler(const size_t passed, const size_t failed, const failure_t failure);

    status_t default_case_set_up_handler   (const Case *const source, const size_t index_of_case);
    status_t default_case_tear_down_handler(const Case *const source, const size_t passed, const size_t failed);
    status_t default_case_failure_handler  (const Case *const source, const failure_t reason);

}
}
}

#endif // MBED_TEST_ASYNC_DEFAULT_HANDLER_H
