// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.
//------------------------------------------------------------------------------
#ifndef INCLUDE_UART_HW_H_
#define INCLUDE_UART_HW_H_
//------------------------------------------------------------------------------
//
// This header specifies the selected STM32H5 UART hardware backend.
//
//------------------------------------------------------------------------------

#include <stdbool.h>
#include <stdint.h>
#include "uart_api.h"
#include "platform_config.h"

//------------------------------------------------------------------------------
// Function Declarations
//------------------------------------------------------------------------------
void uart_hw_install(uart_hw_vtable_t *pv);

//------------------------------------------------------------------------------
// Change baud at run-time
bool uart_hw_reinit(uint32_t baud);

#endif // INCLUDE_UART_HW_H_
