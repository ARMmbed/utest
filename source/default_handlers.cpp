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

#include "utest/default_handlers.h"
#include "utest/case.h"

using namespace utest::v1;

static void selftest_failure_handler(const failure_t failure);
static void test_failure_handler(const failure_t failure);

const handlers_t utest::v1::verbose_continue_handlers = {
    verbose_test_setup_handler,
    verbose_test_teardown_handler,
    test_failure_handler,
    verbose_case_setup_handler,
    verbose_case_teardown_handler,
    verbose_case_failure_handler
};


// --- SPECIAL HANDLERS ---
static void selftest_failure_handler(const failure_t failure) {
    if (failure.location == LOCATION_TEST_SETUP || failure.location == LOCATION_TEST_TEARDOWN || failure.reason == REASON_ASSERTION) {
        verbose_test_failure_handler(failure);
    }
    if (failure.reason == REASON_ASSERTION) {
        printf("{{failure}}\n{{end}}\n");
        while(1) ;
    }
}

static void test_failure_handler(const failure_t failure) {
    if (failure.location == LOCATION_TEST_SETUP || failure.location == LOCATION_TEST_TEARDOWN) {
        verbose_test_failure_handler(failure);
        printf("{{failure}}\n{{end}}\n");
        while(1) ;
    }
}

// --- VERBOSE TEST HANDLERS ---
status_t utest::v1::verbose_test_setup_handler(const size_t number_of_cases)
{
    printf(">>> Running %u test cases...\n", number_of_cases);
    return STATUS_CONTINUE;
}

void utest::v1::verbose_test_teardown_handler(const size_t passed, const size_t failed, const failure_t failure)
{
    printf("\n>>> Test cases: %u passed, %u failed", passed, failed);
    if (failure.reason == REASON_NONE) {
        printf("\n");
    } else  {
        printf(" with reason '%s'\n", stringify(failure.reason));
    }
    if (failed) printf(">>> TESTS FAILED!\n");
}

void utest::v1::verbose_test_failure_handler(const failure_t failure)
{
    printf(">>> failure with reason '%s' during '%s'\n", stringify(failure.reason), stringify(failure.location));
}

// --- VERBOSE CASE HANDLERS ---
status_t utest::v1::verbose_case_setup_handler(const Case *const source, const size_t index_of_case)
{
    printf("\n>>> Running case #%u: '%s'...\n", index_of_case + 1, source->get_description());
    return STATUS_CONTINUE;
}

status_t utest::v1::verbose_case_teardown_handler(const Case *const source, const size_t passed, const size_t failed, const failure_t failure)
{
    printf(">>> '%s': %u passed, %u failed", source->get_description(), passed, failed);
    if (failure.reason == REASON_NONE) {
        printf("\n");
    } else  {
        printf(" with reason '%s'\n", stringify(failure.reason));
    }
    return STATUS_CONTINUE;
}

status_t utest::v1::verbose_case_failure_handler(const Case *const /*source*/, const failure_t failure)
{
    if (!(failure.reason & REASON_ASSERTION)) {
        verbose_test_failure_handler(failure);
    }
    if (failure.reason & (REASON_TEST_TEARDOWN | REASON_CASE_TEARDOWN)) return STATUS_ABORT;
    if (failure.reason & REASON_IGNORE) return STATUS_IGNORE;
    return STATUS_CONTINUE;
}
