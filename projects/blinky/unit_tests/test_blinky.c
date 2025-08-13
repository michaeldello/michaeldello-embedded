// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.
//------------------------------------------------------------------------------
//
// Define Blinky application unit tests
//
//------------------------------------------------------------------------------

#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

//--------------------
// Unit Test Framework
//--------------------
#include <cmocka.h>
//--------------------

#include "blinky.h"
#include "gpio_api.h"

//------------------------------------------------------------------------------
// Function Declarations
//------------------------------------------------------------------------------

// From gpio_stub
uint8_t gpio_get_pin(uint32_t pin);

//------------------------------------------------------------------------------
// Function Definitions
//------------------------------------------------------------------------------

// Reusable Test Setup
static int setup(void **state) {
    (void)state;
    gpio_init();
    blinky_init();
    return 0;
}

//------------------------------------------------------------------------------
// Test Cases
//------------------------------------------------------------------------------
static void test_toggle(void **state) {
    (void)state;
    gpio.write(0, 0);
    assert_int_equal(0, gpio_get_pin(0));
    blinky_toggle();
    assert_int_equal(1, gpio_get_pin(0));
    blinky_toggle();
    assert_int_equal(0, gpio_get_pin(0));
}

//------------------------------------------------------------------------------
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(test_toggle, setup),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}