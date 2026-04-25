// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.
//------------------------------------------------------------------------------

#include <stdint.h>
#include "uart_api.h"
#include "uart_core.h"
#include "platform_config.h"
#include "uart_hw.h"

//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------

#ifndef UART_RX_SIZE
#define UART_RX_SIZE  128
#endif

#ifndef UART_TX_SIZE
#define UART_TX_SIZE  128
#endif

//------------------------------------------------------------------------------
// Opaque Context
//------------------------------------------------------------------------------

// Since context is opaque, size is dependent on backend implementation,
// determined dynamically
extern size_t uart_context_size(void);
static uint8_t uart_context_store[512];

//------------------------------------------------------------------------------
int main(void) {

    // Uncomment if minimal system clock init not covered by startup code
    // SystemInit();

    uart_t *pU = (uart_t*)uart_context_store;

    uint8_t rx_fifo[UART_RX_SIZE];
    uint8_t tx_fifo[UART_TX_SIZE];

    uart_hw_vtable_t hw;
    uart_hw_install(&hw);

    (void)uart_init(
        pU, &hw, 115200, rx_fifo, sizeof(rx_fifo), tx_fifo, sizeof(tx_fifo));

    while (1) {
        //-------------------
        // Polling option
        if (hw.hw_rx_available()) {
            uint8_t byte = hw.hw_rx_read();
            // If USART interrupt enabled at some point, ISR will call this
            // directly, in which case this entire polling option can be removed
            uart_isr_rx_byte(pU, byte);
        }
        // End polling option
        //-------------------
        // Echo the byte and push to hardware
        uart_echo_pump(pU);
        uart_service_tx(pU);
    }
}
