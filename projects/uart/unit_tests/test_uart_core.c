// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.
//------------------------------------------------------------------------------
//
// Define portable UART Core unit tests
//
//------------------------------------------------------------------------------

//--------------------
// Unit Test Framework
//--------------------
#include <cmocka.h>
//--------------------

#include <string.h>
#include "uart_core.h"
#include "uart_hw_stub.h"

//------------------------------------------------------------------------------
// Test Definitions
//------------------------------------------------------------------------------
static void test_echo_basic(void**) {
    uart_t UART;
    uart_hw_vtable_t VTable;
    uart_stub_ctx_t CTX = {0};
    uint8_t rx_src[] = "test";
    uint8_t tx_out[16];
    uint8_t rx_fifo[8];
    uint8_t tx_fifo[8];

    CTX.prx_src = rx_src;
    CTX.rx_len = sizeof(rx_src) - 1;
    CTX.ptx_buf = tx_out;
    CTX.tx_capacity = sizeof(tx_out);
    // Simulate HW is ready
    CTX.tx_bytes = 64;
 
    uart_hw_stub_create(&VTable, &CTX);
    assert_true(
        uart_init(
            &UART, 
            &VTable, 
            115200, 
            rx_fifo, 
            sizeof(rx_fifo), 
            tx_fifo, 
            sizeof(tx_fifo)));

    // Simulate ISR
    for (size_t i = 0; i < CTX.rx_len; i++) {
        uart_isr_rx_byte(&UART, rx_src[i]);
    }
    // Perform echo
    uart_echo_pump(&UART);
    // Flush Tx FIFO to UART
    uart_service_tx(&UART);

    assert_int_equal(CTX.rx_len, CTX.tx_len);
    assert_memory_equal(rx_src, tx_out, CTX.rx_len);
}

//------------------------------------------------------------------------------
static void test_tx_limiting(void**) {
    uart_t UART;
    uart_hw_vtable_t VTable;
    uart_stub_ctx_t CTX = {0};
    uint8_t tx_out[4];
    uint8_t rx_fifo[8];
    uint8_t tx_fifo[4];

    CTX.ptx_buf = tx_out;
    CTX.tx_capacity = sizeof(tx_out);
    // Rate limit Tx
    CTX.tx_bytes = 1;

    uart_hw_stub_create(&VTable, &CTX);
    assert_true(
        uart_init(
            &UART, 
            &VTable, 
            115200, 
            rx_fifo, 
            sizeof(rx_fifo), 
            tx_fifo, 
            sizeof(tx_fifo)));

    const uint8_t msg[] = "TEST";
    size_t enq = uart_write(&UART, msg, sizeof(msg) - 1);
    // Check Tx FIFO enqueued at least some
    assert_true(enq >= 3);

    // Check first attempt only sends one byte
    uart_service_tx(&UART);
    assert_int_equal(1, CTX.tx_len);

    // Relax Tx bytes allowed and try again, checking that can send more now
    CTX.tx_bytes = 10;
    uart_service_tx(&UART);
    assert_true(CTX.tx_len >= 3);
}

//------------------------------------------------------------------------------
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_echo_basic),
        cmocka_unit_test(test_tx_limiting),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}