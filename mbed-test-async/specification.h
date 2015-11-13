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

#ifndef MBED_TEST_ASYNC_SPECIFICATION_H
#define MBED_TEST_ASYNC_SPECIFICATION_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "types.h"
#include "case.h"
#include "default_handlers.h"

namespace mbed {
namespace test {
namespace v0 {

    class Specification
    {
    public:
        template< size_t N >
        Specification(const Case (&cases)[N],
                      const test_tear_down_handler_t tear_down_handler = default_handler,
                      const handlers_t defaults = default_handlers) :
            set_up_handler(default_handler), tear_down_handler(tear_down_handler),
            cases(cases), length(N),
            defaults(defaults)
        {}

        template< size_t N >
        Specification(const Case (&cases)[N], const handlers_t defaults) :
            set_up_handler(default_handler), tear_down_handler(default_handler),
            cases(cases), length(N),
            defaults(defaults)
        {}

        template< size_t N >
        Specification(const test_set_up_handler_t set_up_handler,
                      const Case (&cases)[N],
                      const test_tear_down_handler_t tear_down_handler = default_handler,
                      const handlers_t defaults = default_handlers) :
            set_up_handler(set_up_handler), tear_down_handler(tear_down_handler),
            cases(cases), length(N),
            defaults(defaults)
        {}

        template< size_t N >
        Specification(const test_set_up_handler_t set_up_handler,
                      const Case (&cases)[N],
                      const handlers_t defaults) :
            set_up_handler(set_up_handler), tear_down_handler(default_handler),
            cases(cases), length(N),
            defaults(defaults)
        {}

    private:
        const test_set_up_handler_t set_up_handler;
        const test_tear_down_handler_t tear_down_handler;
        const Case *const cases;
        const size_t length;
        const handlers_t defaults;

        friend class Harness;
    };

}
}
}

 #endif // MBED_TEST_ASYNC_SPECIFICATION_H
