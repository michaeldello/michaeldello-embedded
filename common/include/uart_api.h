// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.
//------------------------------------------------------------------------------
#ifndef INCLUDE_UART_API_H_
#define INCLUDE_UART_API_H_
//------------------------------------------------------------------------------
//
// This header specifies a generic UART API that can be implemented for various
// deployment contexts, e.g., execution on a target hardware device, or unit 
// testing on a development host. In the latter case, the implementation will be
// a stub.
//
//------------------------------------------------------------------------------

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

//------------------------------------------------------------------------------
// Types
//------------------------------------------------------------------------------

// Hardware API
// Functions to be called by portable core
typedef struct {
    bool (*hw_init)(uint32_t baud);
    bool (*hw_tx_ready)(void);
    void (*hw_tx_write)(uint8_t byte);
    // Polling Rx design:
    // Read one byte if there a byte ready
    bool (*hw_rx_available)(void);
    uint8_t (*hw_rx_read)(void);
} uart_hw_vtable_t;

// One-per-instance opaque handle
typedef struct uart_t uart_t;
// Helper function to query uart_t size for storage allocation
size_t uart_context_size(void);

//------------------------------------------------------------------------------
// Function Declarations
//------------------------------------------------------------------------------
// Portable core
bool uart_init(
    uart_t *pu, 
    const uart_hw_vtable_t *phw, 
    uint32_t baud, 
    void *prx_buf, 
    size_t rx_size, 
    void *ptx_buf, 
    size_t tx_size);

//------------------------------------------------------------------------------
// ISR Rx Variant: Push inbound byte into FIFO from ISR
void uart_isr_rx_byte(uart_t *pu, uint8_t byte);

//------------------------------------------------------------------------------
// Context: Main Loop or TX Empty ISR
// Move queued TX bytes to HW
void uart_service_tx(uart_t *pu);

//------------------------------------------------------------------------------
// Context: Application APIs
size_t uart_write(uart_t *pu, const uint8_t *pdata, size_t len);
size_t uart_tx_queued(const uart_t *pu);
// Polling Rx Variant: Check if Rx available, then read
size_t uart_rx_available(const uart_t *pu);
size_t uart_read(uart_t *pu, uint8_t *pout, size_t maxlen);

//------------------------------------------------------------------------------
// Echo Helper
void uart_echo_pump(uart_t *pu);

//------------------------------------------------------------------------------
// Overflow Diag
uint32_t uart_rx_overflow_count(const uart_t *pu);
void uart_rx_overflow_clear(uart_t *pu);

//------------------------------------------------------------------------------
// Override Drain Chunk Size
void uart_set_echo_chunk_size(uart_t *pu, size_t chunk_size_bytes);
size_t uart_get_echo_chunk_size(const uart_t *pu);

#endif // INCLUDE_UART_API_H_