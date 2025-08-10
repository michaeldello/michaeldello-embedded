// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.
//------------------------------------------------------------------------------

#include "blinky.h"
#include "gpio_api.h"

//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------

#define LED_PIN 0

void blinky_init(void)
{
    gpio_init();
}

void blinky_toggle(void)
{
    gpio.toggle(LED_PIN);
}