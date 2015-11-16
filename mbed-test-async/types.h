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
        CONTROL_FLOW_NEXT   = 0,    ///< continue with the next test case
        CONTROL_FLOW_REPEAT,        ///< repeat the current test case
    };

    enum status_t {
        STATUS_CONTINUE = 0,        ///< continues testing
        STATUS_ABORT,               ///< stops testing
    };

    enum failure_t {
        FAILURE_NONE = 0,   ///< No failure occurred
        FAILURE,            ///< An unknown failure occurred
        FAILURE_CASES,      ///< A failure occurred in at least one test case
        FAILURE_EMPTY_CASE, ///< The test case contains only empty handlers
        FAILURE_SETUP,      ///< A failure occurred on setup
        FAILURE_TEARDOWN,   ///< A failure occurred on teardown
        FAILURE_TIMEOUT,    ///< An expected asynchronous call timed out
        FAILURE_ASSERTION,  ///< An assertion failed
    };

    class Case; // forward declaration

    /** @brief Test setup handler.
     *
     * This handler is called before execution of any test case and
     * allows you to initialize your common test environment.
     *
     * @param   number_of_cases the total number of test cases in the test specification
     * @returns You can return `STATUS_ABORT` if you initialization failed and the test teardown handler will
     *          then be called with the `FAILURE_SETUP`.
     */
    typedef status_t (*test_set_up_handler_t)(const size_t number_of_cases);

    /** @brief Test teardown handler.
     *
     * This handler is called after execution of all test case or if test execution is aborted.
     * You can use this handler to de-initialize your test environment and output test statistics.
     * The failure argument contains the immediate reason why this handler is called.
     * If the test completed normally without failures, this will contain `FAILURE_NONE`.
     *
     * After execution of this handler, the test harness will stop execution.
     *
     * @param   passed  the number of cases without failures
     * @param   failed  the number of cases with at least one failure
     * @param   failure the reason why this handler was called
     */
    typedef void (*test_tear_down_handler_t)(const size_t passed, const size_t failed, const failure_t failure);

    /** @brief Test case setup handler.
     *
     * This handler is called before execution of each test case and
     * allows you to modify your environment before each test case.
     *
     * @param   source          the test case to be setup
     * @param   index_of_case   the current index of the test case within the specification
     * @returns You can return `STATUS_ABORT` to indicate that your setup failed, which will call the case
     *          failure handler with `FAILURE_SETUP` and then the case teardown handler with `FAILURE_SETUP`.
     *          This gives the teardown handler a chance to clean up a failed setup.
     */
    typedef status_t (*case_set_up_handler_t)(const Case *const source, const size_t index_of_case);

    /** @brief Test case handler
     *
     * This handler is called only if the case setup succeeded and is followed by the test case teardown handler.
     * @note This handler is executed only once.
     */
    typedef void (*case_handler_t)(void);

    /** @brief Test case handler (repeatable)
     *
     * This handler is called only if the case setup succeeded and is eventually followed by the test case teardown handler.
     *
     * @param   repeat_count    starting at `0`, contains the number of times this handler has been called
     * @returns You can return `CONTROL_FLOW_REPEAT` to repeat the same test case again.
     */
    typedef control_flow_t (*case_control_flow_handler_t)(const size_t repeat_count);

    /** @brief Test case teardown handler.
     *
     * This handler is called after execution of each test case or all repeated test cases and
     * allows you to reset your environment after each test case.
     *
     * @param   source  the test case to be torn down
     * @param   passed  the number of cases without failures (can be >1 for repeated test cases)
     * @param   failed  the number failures (can be larger than the number of (repeated) test cases)
     * @param   failure the reason why this handler was called
     * @returns You can return `STATUS_ABORT` to indicate that your teardown failed, which will call the case
     *          failure handler with `FAILURE_TEARDOWN`.
     */
    typedef status_t (*case_tear_down_handler_t)(const Case *const source, const size_t passed, const size_t failed, const failure_t reason);

    /** @brief Test case failure handler.
     *
     * This handler is called whenever a failure occurred during the setup, execution or teardown.
     *
     * @param   source  the test case in which the failure occurred
     * @param   reason the reason why this handler was called
     * @returns You can return `STATUS_ABORT` to indicate that this failure is non-recoverable, which will call the
     *          case teardown handler with reason. If a failure occurs during teardown, the teardown will not be called again.
     */
    typedef status_t (*case_failure_handler_t)(const Case *const source, const failure_t reason);

}
}
}

#endif // MBED_TEST_ASYNC_TYPES_H
