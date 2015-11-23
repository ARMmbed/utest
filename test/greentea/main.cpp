
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
#include "mbed-drivers/test_env.h"


using namespace utest::v0;

control_t test_repeat(const size_t repeat_count)
{
    printf("Called for the %u. time\n", repeat_count+1);

    return (repeat_count < 5) ? CaseRepeatHandlerOnly : CaseNext;
}

void test_assert_fail()
{
    printf("failing.\n");
    Harness::raise_failure(FAILURE_ASSERTION);
}


status_t greentea_setup(const size_t number_of_cases)
{
    MBED_HOSTTEST_TIMEOUT(20);
    MBED_HOSTTEST_SELECT(default_auto);
    MBED_HOSTTEST_DESCRIPTION(Basic);
    MBED_HOSTTEST_START("ASYNC_TEST_GREENTEA_ABORT");

    return verbose_test_setup_handler(number_of_cases);
}

Case cases[] =
{
    Case("test repeats (success)", test_repeat),
    Case("test assert (fail)", test_assert_fail),
};

Specification specification(greentea_setup, cases);

void app_start(int, char*[]) {
    static Serial pc(USBTX, USBRX);
    pc.baud(115200);

    Harness::run(specification);
}
