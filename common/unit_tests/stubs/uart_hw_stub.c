// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.
//------------------------------------------------------------------------------
//
// UART API implementation for unit testing application code
//
//------------------------------------------------------------------------------

#include "uart_hw_stub.h"

//------------------------------------------------------------------------------
// Stubbed UART object
//------------------------------------------------------------------------------

static uart_stub_ctx_t *pGlobalctx;

//------------------------------------------------------------------------------
// Stub Function Definitions
//------------------------------------------------------------------------------
static bool s_init(uint32_t baud) {
    // No-Op, but use the parameter
    (void)baud;
    return true;
}

//------------------------------------------------------------------------------
static bool s_tx_ready(void) {
    // Any bytes not sent?
    return pGlobalctx->tx_bytes > 0;
}

//------------------------------------------------------------------------------
static void s_tx_write(uint8_t byte) {
    // Write the byte if there is room
    if (pGlobalctx->tx_len < pGlobalctx->tx_capacity) {
        pGlobalctx->tx_buf[pGlobalctx->tx_len++] = byte;
    }
    // Apply flow control
    if (pGlobalctx->tx_bytes > 0) { 
        pGlobalctx->tx_bytes--;
    }
}

//------------------------------------------------------------------------------
static bool s_rx_available(void) {
    // More Rx data to get?
    return pGlobalctx->rx_idx < pGlobalctx->rx_len;
}

//------------------------------------------------------------------------------
static uint8_t s_rx_read(void) {
    // Return the next Rx byte, if any, otherwise return 0
    return (pGlobalctx->rx_idx < pGlobalctx->rx_len) ? 
        (pGlobalctx->rx_src[pGlobalctx->rx_idx++]) : 0;
}

//------------------------------------------------------------------------------
// Function Definitions
//------------------------------------------------------------------------------
void uart_hw_stub_create(uart_hw_vtable_t *pv, uart_stub_ctx_t *pctx) {
    pGlobalctx = pctx;
    // Install stub implementation
    pv->hw_init = s_init;
    pv->hw_tx_ready = s_tx_ready;
    pv->hw_tx_write = s_tx_write;
    pv->hw_rx_available = s_rx_available;
    pv->hw_rx_read = s_rx_read;
}
