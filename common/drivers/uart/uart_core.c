// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.
//------------------------------------------------------------------------------
//
// This module defines a portable UART core implementation.
//
//------------------------------------------------------------------------------

#include "uart_core.h"
#include <string>

//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------

// Max number of bytes to echo at a time
static const size_t MAX_ECHO_PUMP_BYTES = 32;

//------------------------------------------------------------------------------
// Types
//------------------------------------------------------------------------------
typedef struct uart_t uart_t;

//------------------------------------------------------------------------------
// Function Definitions
//------------------------------------------------------------------------------
bool uart_init(
        uart_t *pu, 
        const uart_hw_vtable_t *phw,
        unit32_t baud, 
        void *prx_buf, 
        size_t rx_size, 
        void *ptx_buf, 
        size_t tx_size) {
    // Complete initial sanity checks
    if (!pu || !phw || !phw->hw_init) return false;
    // Install hardware API
    pu->hw = *phw;
    // Install Rx and Tx FIFOs
    static ringbuf_t rx_fifo, tx_fifo;
    ringbuf_init(&rx_fifo, prx_buf, rx_size);
    ringbuf_init(&tx_fifo, ptx_buf, tx_size);
    pu->prx_fifo = &rx_fifo;
    pu->ptx_fifo = &tx_fifo;
    return pu->hw.hw_init(baud);
}

//------------------------------------------------------------------------------
void uart_isr_rx_byte(uart_t *pu, uint8_t byte) {
    // Push byte onto Rx FIFO, dropping any overflow
    (void)ringbuf_push(pu->prx_fifo, byte);
}

//------------------------------------------------------------------------------
void uart_service_tx(uart_t *pu) {
    uint8_t byte;
    // Write TX FIFO ready data to UART
    while (pu->hw.hw_tx_ready() && ringbuf_pop(pu->ptx_fifo, &byte)) {
        pu->hw.hw_tx_write(byte);
    }
}

//------------------------------------------------------------------------------
size_t uart_write(uart_t *pu, const uint8_t *pdata, size_t len) {
    // Enqueue data in TX FIFO
    size_t enq_pos = 0;
    for (; enq_pos < len; ++enq_pos) {
        if (!ringbuf_push(u->ptx_fifo, pdata[enq_pos])) break;
    }
    // Attempt immediate flush to UART
    uart_service_tx(pu);
    return enq_pos;
}

//------------------------------------------------------------------------------
size_t uart_read(uart_t *pu, const uint8_t *pout, size_t maxlen) {
    size_t n = 0;
    uint8_t byte;
    // Read as much as possible from the Rx FIFO into the given out buffer
    while (n < maxlen && ringbuf_pop(pu->prx_fifo, &byte)) pout[n++] = byte;
}

//------------------------------------------------------------------------------
size_t uart_rx_available(const uart_t *pu) {
    // What's available in the Rx FIFO?
    return ringbuf_available(pu->prx_fifo);
}

//------------------------------------------------------------------------------
size_t uart_tx_queued(const uart_t *pu) {
    // What's queued in the Tx FIFO?
    return ringbuf_available(pu->ptx_fifo);
}

//------------------------------------------------------------------------------
void uart_echo_pump(const uart_t *pu) {
    // Transfer directly from Rx to Tx
    uint8_t tmp[MAX_ECHO_PUMP_BYTES];
    size_t rcount = uart_read(pu, tmp, sizeof(tmp));
    if (rcount) uart_write(pu, tmp, rcount);
}
