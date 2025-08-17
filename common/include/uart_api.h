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

typedef struct {
    // HW-specific functions called by portable core
    bool (*hw_init)(uint32_t baud);
    bool (*hw_tx_ready)(void);
    bool (*hw_tx_write)(uint8_t byte);
    bool (*hw_rx_available)(void);
    bool (*hw_rx_read)(void);
} uart_hw_vtable_t;

// Opaque instance handle
// Defer definition to implementation file
typedef struct uart_t {
    uart_hw_vtable_t hw;
    struct ringbuf_t *prx_fifo;
    struct ringbuf_t *ptx_fifo;
} uart_t;

//------------------------------------------------------------------------------
// Function Declarations
//------------------------------------------------------------------------------
// Portable core
bool uart_init(
    uart_t *pu, 
    const uart_hw_vtable_t *phw, 
    unit32_t baud, 
    void *prx_buf, 
    size_t rx_size, 
    void *ptx_buf, 
    size_t tx_size);

//------------------------------------------------------------------------------
// Push inbound byte from ISR
void uart_isr_rx_byte(uart_t *pu, uint8_t byte);

//------------------------------------------------------------------------------
// Context: Main Loop or TX Empty ISR
// Move queued TX bytes to HW
void uart_service_tx(uart_t *pu);

//------------------------------------------------------------------------------
// Context: Application APIs
size_t uart_write(uart_t *pu, const uint8_t *pdata, size_t len);
size_t uart_read(uart_t *pu, const uint8_t *pout, size_t maxlen);
size_t uart_rx_available(const uart_t *pu);
size_t uart_tx_queued(const uart_t *pu);

//------------------------------------------------------------------------------
// Echo Helper
void uart_echo_pump(const uart_t *pu);

#endif // INCLUDE_UART_API_H_