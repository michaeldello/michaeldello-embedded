// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.
//------------------------------------------------------------------------------
#ifndef INCLUDE_UART_HW_STUB_H_
#define INCLUDE_UART_HW_STUB_H_
//------------------------------------------------------------------------------
//
// UART stub specification for unit testing application code
//
//------------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>
#include "uart_api.h"

//------------------------------------------------------------------------------
// Types
//------------------------------------------------------------------------------

// Context
typedef struct {
    // Simulate Tx
    uint8_t *ptx_buf;
    size_t tx_capacity;
    size_t tx_len;
    // Simulate Rx
    const uint8_t *prx_src;
    size_t rx_len;
    size_t rx_idx;
    // Simulate flow control (how many bytes HW is ready to send)
    int tx_bytes;
} uart_stub_ctx_t;

//------------------------------------------------------------------------------
// Function Declarations
//------------------------------------------------------------------------------
void uart_hw_stub_create(uart_hw_vtable_t *pv, uart_stub_ctx_t *pctx);

#endif // INCLUDE_UART_HW_STUB_H_