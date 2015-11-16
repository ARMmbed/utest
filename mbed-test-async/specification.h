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

    /** @brief Test specification containing the setup and teardown handlers and test cases.
     *
     * This class simply holds the test cases and allows you to specify default handlers, and
     * override setup and teardown handlers.
     * The order of arguments is:
     *  - test setup handler (optional)
     *  - array of test cases (required)
     *  - test teardown handler (optional)
     *  - default handlers (optional)
     */
    class Specification
    {
    public:
        template< size_t N >
        Specification(const Case (&cases)[N],
                      const test_teardown_handler_t teardown_handler = default_handler,
                      const handlers_t defaults = default_handlers) :
            setup_handler(default_handler), teardown_handler(teardown_handler),
            cases(cases), length(N),
            defaults(defaults)
        {}

        template< size_t N >
        Specification(const Case (&cases)[N], const handlers_t defaults) :
            setup_handler(default_handler), teardown_handler(default_handler),
            cases(cases), length(N),
            defaults(defaults)
        {}

        template< size_t N >
        Specification(const test_setup_handler_t setup_handler,
                      const Case (&cases)[N],
                      const test_teardown_handler_t teardown_handler = default_handler,
                      const handlers_t defaults = default_handlers) :
            setup_handler(setup_handler), teardown_handler(teardown_handler),
            cases(cases), length(N),
            defaults(defaults)
        {}

        template< size_t N >
        Specification(const test_setup_handler_t setup_handler,
                      const Case (&cases)[N],
                      const handlers_t defaults) :
            setup_handler(setup_handler), teardown_handler(default_handler),
            cases(cases), length(N),
            defaults(defaults)
        {}

    private:
        const test_setup_handler_t setup_handler;
        const test_teardown_handler_t teardown_handler;
        const Case *const cases;
        const size_t length;
        const handlers_t defaults;

        friend class Harness;
    };

}
}
}

 #endif // MBED_TEST_ASYNC_SPECIFICATION_H
