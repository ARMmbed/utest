
/* mbed Microcontroller Library
 * Copyright (c) 2013-2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "mbed-drivers/mbed.h"
#include "mbed-test-async/utest.h"


using namespace mbed::test::v0;

control_t test_repeat(const size_t repeat_count)
{
    printf("Called for the %u. time\n", repeat_count+1);

    return (repeat_count < 5) ? CaseRepeatHandlerOnly : CaseNext;
}

void test_assert_success()
{
    printf("passing.\n");
}

void test_assert_fail()
{
    printf("failing.\n");
    Harness::raise_failure(FAILURE_ASSERTION);
}

control_t test_async_fail()
{
    printf("not generating an asynchronous callback.\n");
    return CaseTimeout(200);
}

void test_async_validate()
{
    printf("asynchronous callback.\n");
    Harness::validate_callback();
}

control_t test_async_success()
{
    printf("generating an asynchronous callback.\n");
    minar::Scheduler::postCallback(test_async_validate).delay(minar::milliseconds(500)).tolerance(0);
    return CaseTimeout(1000);
}

void test_async_validate_assert_fail()
{
    printf("failing.\n");
    Harness::raise_failure(FAILURE_ASSERTION);
    printf("failing again.\n");
    Harness::raise_failure(FAILURE_ASSERTION);
    Harness::validate_callback();
}

control_t test_async_callback_assert_fail()
{
    minar::Scheduler::postCallback(test_async_validate_assert_fail).delay(minar::milliseconds(500)).tolerance(0);
    return CaseTimeout(1000);
}

Case cases[] =
{
    Case("NULL test (fail)", ignore_handler, (case_handler_t)ignore_handler, ignore_handler),
    Case("test repeats (success)", test_repeat),
    Case("test assert (fail)", test_assert_fail),
    Case("test async (fail)", test_async_fail),
    Case("test async (success)", test_async_success),
    Case("test async callback (fail)", test_async_callback_assert_fail),
    Case("printf with integer formatting (success)", test_assert_success),
};

Specification specification(cases, verbose_continue_handlers);


void app_start(int, char*[]) {
    static Serial pc(USBTX, USBRX);
    pc.baud(115200);

    Harness::run(specification);
}
