// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.
//------------------------------------------------------------------------------

#include "blinky.h"
#include <stdint.h>

//------------------------------------------------------------------------------
void delay(void) {
    // Simple delay ... TBI - replace with SysTick
    for (volatile uint32_t i = 0; i < 300000; ++i) {
        __asm__ volatile("nop");
    }
}

//------------------------------------------------------------------------------
int main(void) {
    blinky_init();
    while (1) {
        blinky_toggle();
        delay();
    }
}