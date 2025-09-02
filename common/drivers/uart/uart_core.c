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
#include <string.h>

//------------------------------------------------------------------------------
// Types
//------------------------------------------------------------------------------

// Opaque handle declared in API header
struct uart_t {
    // Hardware backend - to be installed
    uart_hw_vtable_t hw;
    // Each instance has its own statically allocated FIFOs
    ringbuf_t rx_fifo;
    ringbuf_t tx_fifo;
    uint32_t  rx_overflow_count;
    size_t    echo_chunk_size_bytes;
};
// Define uart_t size helper function
size_t uart_context_size(void) { return sizeof(struct uart_t); }

//------------------------------------------------------------------------------
// Function Definitions
//------------------------------------------------------------------------------
bool uart_init(
        uart_t                 *pu, 
        const uart_hw_vtable_t *phw,
        uint32_t               baud, 
        void                   *prx_buf, 
        size_t                 rx_size, 
        void                   *ptx_buf, 
        size_t                 tx_size) {
    // Initial sanity checks
    if (!pu || !phw || !phw->hw_init ||
            !prx_buf ||  !ptx_buf || !rx_size || ! tx_size) {
        return false;
    }
    // Install hardware API
    pu->hw = *phw;
    ringbuf_init(&pu->rx_fifo, prx_buf, rx_size);
    ringbuf_init(&pu->tx_fifo, ptx_buf, tx_size);
    pu->rx_overflow_count = 0;
    pu->echo_chunk_size_bytes = UART_ECHO_DRAIN_CHUNK_BYTES;
    return pu->hw.hw_init(baud);
}

//------------------------------------------------------------------------------
void uart_isr_rx_byte(uart_t *pu, uint8_t byte) {
    // To be called from ISR (or test shim)
    // Push byte onto Rx FIFO, dropping any overflow
    // Maintain diagnostics for data loss
    // Notes:
    //    - A real-time (lossy) application could keep latest instead 
    //      of dropping it, dropping oldest (pop) instead
    if (!ringbuf_push(&pu->rx_fifo, byte)) {
        // FIFO full
        pu->rx_overflow_count++;
    }
}

//------------------------------------------------------------------------------
void uart_service_tx(uart_t *pu) {
    uint8_t byte;
    // Write TX FIFO ready data to UART
    while (pu->hw.hw_tx_ready() && ringbuf_pop(&pu->tx_fifo, &byte)) {
        pu->hw.hw_tx_write(byte);
    }
}

//------------------------------------------------------------------------------
size_t uart_write(uart_t *pu, const uint8_t *pdata, size_t len) {
    // Enqueue data in TX FIFO
    // Persist position value, returned later
    size_t enq_pos = 0;
    for (; enq_pos < len; ++enq_pos) {
        if (!ringbuf_push(&pu->tx_fifo, pdata[enq_pos])) {
            break;
        }
    }
    // Attempt immediate flush to UART
    uart_service_tx(pu);
    return enq_pos;
}

//------------------------------------------------------------------------------
size_t uart_rx_available(const uart_t *pu) {
    // For polling the UART: what's available in the Rx FIFO?
    return ringbuf_available(&pu->rx_fifo);
}

//------------------------------------------------------------------------------
size_t uart_read(uart_t *pu, uint8_t *pout, size_t maxlen) {
    size_t n = 0;
    uint8_t byte;
    // Read as much as possible from the Rx FIFO into the given out buffer
    while (n < maxlen && ringbuf_pop(&pu->rx_fifo, &byte)) {
        pout[n++] = byte;
    }
    return n;
}

//------------------------------------------------------------------------------
size_t uart_tx_queued(const uart_t *pu) {
    // What's queued in the Tx FIFO?
    return ringbuf_available(&pu->tx_fifo);
}

//------------------------------------------------------------------------------
void uart_echo_pump(uart_t *pu) {
    // Transfer directly from Rx to Tx in configured chunks
    size_t chunk = 
        pu->echo_chunk_size_bytes ? 
            pu->echo_chunk_size_bytes : UART_ECHO_DRAIN_CHUNK_BYTES;
    while (uart_rx_available(pu)) {
        size_t still_to_read = uart_rx_available(pu);
        if (still_to_read > chunk) {
            still_to_read = chunk;
        }
        // Allocate upper bound on stack
        uint8_t tmp[UART_ECHO_DRAIN_CHUNK_BYTES];
        size_t rcount = uart_read(pu, tmp, still_to_read);
        if (!rcount) break;
        uart_write(pu, tmp, rcount);
    }
}

//------------------------------------------------------------------------------
uint32_t uart_rx_overflow_count(const uart_t *pu) {
    return pu->rx_overflow_count;
}

//------------------------------------------------------------------------------
void uart_rx_overflow_clear(uart_t *pu) {
    pu->rx_overflow_count = 0;
}

//------------------------------------------------------------------------------
void uart_set_echo_chunk_size(uart_t *pu, size_t chunk_size_bytes) {
    pu->echo_chunk_size_bytes = chunk_size_bytes;
}

//------------------------------------------------------------------------------
size_t uart_get_echo_chunk_size(const uart_t *pu) {
    return pu->echo_chunk_size_bytes;
}