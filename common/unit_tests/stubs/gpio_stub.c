// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.
//------------------------------------------------------------------------------
//
// GPIO API implementation for unit testing application code
//
//------------------------------------------------------------------------------

#include "gpio_api.h"
#include <stdio.h>

//------------------------------------------------------------------------------
// API Instantiation
//------------------------------------------------------------------------------

gpio_api_t gpio;

//------------------------------------------------------------------------------
// Variables
//------------------------------------------------------------------------------

static uint8_t pin_states[32] = {0};

//------------------------------------------------------------------------------
// Function Definitions
//------------------------------------------------------------------------------
void gpio_write(uint32_t pin, uint8_t value) {
    pin_states[pin] = value;
}

//------------------------------------------------------------------------------
void gpio_toggle(uint32_t pin) {
    pin_states[pin] ^= 1U;
}

//------------------------------------------------------------------------------
void gpio_init(void) {
    // Install API implementation
    gpio.write  = gpio_write;
    gpio.toggle = gpio_toggle;
}

//------------------------------------------------------------------------------
uint8_t gpio_get_pin(uint32_t pin) {
    return pin_states[pin];
}
