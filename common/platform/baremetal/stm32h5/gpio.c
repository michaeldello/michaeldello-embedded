// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.
//------------------------------------------------------------------------------
//
// This module defines the common GPIO API for a bare metal implementation on an
// STM32H5 platform.
//
//------------------------------------------------------------------------------

#include "gpio_api.h"
#include "platform_config.h"

//------------------------------------------------------------------------------
// API Instantiation
//------------------------------------------------------------------------------

gpio_api_t gpio;

//------------------------------------------------------------------------------
// Function Definitions
//------------------------------------------------------------------------------
void gpio_write(uint32_t pin, uint8_t value) {
    if (value) {
        // Set pin value
        GPIOB_ODR |= (1U << pin);
    } else {
        // Clear pin value
        GPIOB_ODR &= ~(1U << pin);
    }
}

//------------------------------------------------------------------------------
void gpio_toggle(uint32_t pin) {
    GPIOB_ODR ^= (1U << pin);
}

//------------------------------------------------------------------------------
void gpio_init(void) {
    // Enable GPIOB Clock (pin 1 in RCC) 
    RCC_AHB4ENR |= (1 << 1);

    // Configure PB0 as output
    //
    // Clear mode pin 0 (bits 0..1)
    GPIOB_MODER &= ~(3U);
    // Set mode to output
    GPIOB_MODER |= (1U);

    // Install API implementation
    gpio.write  = gpio_write;
    gpio.toggle = gpio_toggle;
}
