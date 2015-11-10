
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
#include "mbed-test-async/async_test.h"


using namespace mbed::test::v0;

status_t test_printf()
{
    static int counter = 0;
    printf("Called for the %u. time\n", ++counter);
    return STATUS_SUCCESS;
}

status_t test_assert_success()
{
    TEST_ASSERT_EQUAL(0, 0);
    return STATUS_SUCCESS;
}

status_t test_assert_fail()
{
    TEST_ASSERT_EQUAL(0, 1);
    return STATUS_SUCCESS;
}

status_t test_async_fail()
{
    return STATUS_EXPECT_ASYNC_CALL;
}

void test_async_validate()
{
    TestHarness::validateCallback();
}

status_t test_async_success()
{
    minar::Scheduler::postCallback(test_async_validate).delay(minar::milliseconds(500)).tolerance(0);
    return STATUS_EXPECT_ASYNC_CALL;
}

void test_async_validate_assert_fail()
{
    TEST_ASSERT_NOT_EQUAL(0, 0);    // this messes up the stack, due to longjmp usage
    TestHarness::validateCallback();
}

status_t test_async_callback_assert_fail()
{
    minar::Scheduler::postCallback(test_async_validate_assert_fail).delay(minar::milliseconds(500)).tolerance(0);
    return STATUS_EXPECT_ASYNC_CALL;
}

Test specification[] =
{
    Case("test repeats (success)", test_printf, 3),
    Case("test assert (fail)", test_assert_fail),
    AsyncCase("test async (fail)", test_async_fail, 200),
    AsyncCase("test async (success)", test_async_success, 1000),
    AsyncCase("test async callback (fail)", test_async_callback_assert_fail, 1000),
    Case("printf with integer formatting", test_assert_success),
};


void app_start(int, char*[]) {
    static Serial pc(USBTX, USBRX);
    pc.baud(115200);

    TestHarness::run(specification);
}
