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

#ifndef MBED_TEST_ASYNC_HARNESS_H
#define MBED_TEST_ASYNC_HARNESS_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "types.h"
#include "case.h"
#include "default_handlers.h"
#include "specification.h"


namespace mbed {
namespace test {
namespace v0 {

    class Harness
    {
    public:
        static void run(const Specification specification);
        static bool is_busy();

        static void validate_callback();
        static void raise_failure(failure_t reason);

    protected:
        static void run_next_case();
        static void handle_timeout();
        static void schedule_next_case();
    };

}
}
}

#endif // MBED_TEST_ASYNC_HARNESS_H
