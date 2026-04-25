// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.
//------------------------------------------------------------------------------
//
// Define portable UART Core unit tests
//
//------------------------------------------------------------------------------

#include <string.h>
#include <stdarg.h>
#include <setjmp.h>
//--------------------
// Unit Test Framework
//--------------------
#include <cmocka.h>
//--------------------
#include "uart_core.h"
#include "uart_hw_stub.h"

//------------------------------------------------------------------------------
// Test Helpers
//------------------------------------------------------------------------------
static bool test_hw_init(uint32_t baud) {
    (void)baud;
    return true;
}

//------------------------------------------------------------------------------
static bool test_hw_ready(void) {
    return true;
}

//------------------------------------------------------------------------------
static void test_hw_write(uint8_t byte) {
    (void)byte;
}

//------------------------------------------------------------------------------
static bool test_hw_available(void) {
    return false;
}

//------------------------------------------------------------------------------
static uint8_t test_hw_read(void) {
    return 0u;
}

//------------------------------------------------------------------------------
// Test Definitions
//------------------------------------------------------------------------------
static void test_init_validation(void **state) {
    (void)state;  // silence unused warning
    size_t usize = uart_context_size();
    uint8_t ustore[usize];
    uart_t *pUART = (uart_t*)ustore;
    uart_hw_vtable_t VTable;
    uart_stub_ctx_t CTX = {0};
    uint8_t rx_fifo[8];
    uint8_t tx_fifo[8];

    uart_hw_stub_create(&VTable, &CTX);

    assert_false(
        uart_init(
            NULL,
            &VTable,
            115200,
            rx_fifo,
            sizeof(rx_fifo),
            tx_fifo,
            sizeof(tx_fifo)));

    assert_false(
        uart_init(
            pUART,
            NULL,
            115200,
            rx_fifo,
            sizeof(rx_fifo),
            tx_fifo,
            sizeof(tx_fifo)));

    uart_hw_vtable_t invalid = VTable;
    invalid.hw_init = NULL;
    assert_false(
        uart_init(
            pUART,
            &invalid,
            115200,
            rx_fifo,
            sizeof(rx_fifo),
            tx_fifo,
            sizeof(tx_fifo)));

    assert_false(
        uart_init(
            pUART,
            &VTable,
            115200,
            NULL,
            sizeof(rx_fifo),
            tx_fifo,
            sizeof(tx_fifo)));

    assert_false(
        uart_init(
            pUART,
            &VTable,
            115200,
            rx_fifo,
            0u,
            tx_fifo,
            sizeof(tx_fifo)));

    assert_false(
        uart_init(
            pUART,
            &VTable,
            115200,
            rx_fifo,
            sizeof(rx_fifo),
            NULL,
            sizeof(tx_fifo)));

    assert_false(
        uart_init(
            pUART,
            &VTable,
            115200,
            rx_fifo,
            sizeof(rx_fifo),
            tx_fifo,
            0u));
}

//------------------------------------------------------------------------------
static void test_init_validation_requires_full_vtable(void **state) {
    (void)state;  // silence unused warning
    size_t usize = uart_context_size();
    uint8_t ustore[usize];
    uart_t *pUART = (uart_t*)ustore;
    uint8_t rx_fifo[8];
    uint8_t tx_fifo[8];

    uart_hw_vtable_t VTable = {
        .hw_init = test_hw_init,
        .hw_tx_ready = NULL,
        .hw_tx_write = test_hw_write,
        .hw_rx_available = test_hw_available,
        .hw_rx_read = test_hw_read,
    };

    assert_false(
        uart_init(
            pUART,
            &VTable,
            115200,
            rx_fifo,
            sizeof(rx_fifo),
            tx_fifo,
            sizeof(tx_fifo)));

    VTable.hw_tx_ready = test_hw_ready;
    VTable.hw_tx_write = NULL;
    assert_false(
        uart_init(
            pUART,
            &VTable,
            115200,
            rx_fifo,
            sizeof(rx_fifo),
            tx_fifo,
            sizeof(tx_fifo)));

    VTable.hw_tx_write = test_hw_write;
    VTable.hw_rx_available = NULL;
    assert_false(
        uart_init(
            pUART,
            &VTable,
            115200,
            rx_fifo,
            sizeof(rx_fifo),
            tx_fifo,
            sizeof(tx_fifo)));

    VTable.hw_rx_available = test_hw_available;
    VTable.hw_rx_read = NULL;
    assert_false(
        uart_init(
            pUART,
            &VTable,
            115200,
            rx_fifo,
            sizeof(rx_fifo),
            tx_fifo,
            sizeof(tx_fifo)));
}

//------------------------------------------------------------------------------
static void test_echo_basic(void **state) {
    (void)state;  // silence unused warning
    size_t usize = uart_context_size();
    uint8_t ustore[usize];
    uart_t *pUART = (uart_t*)ustore;
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
            pUART, 
            &VTable, 
            115200, 
            rx_fifo, 
            sizeof(rx_fifo), 
            tx_fifo, 
            sizeof(tx_fifo)));

    // Simulate ISR
    for (size_t i = 0; i < CTX.rx_len; i++) {
        uart_isr_rx_byte(pUART, rx_src[i]);
    }
    // Perform echo
    uart_echo_pump(pUART);
    // Flush Tx FIFO to UART
    uart_service_tx(pUART);

    assert_int_equal(CTX.rx_len, CTX.tx_len);
    assert_memory_equal(rx_src, tx_out, CTX.rx_len);
}

//------------------------------------------------------------------------------
static void test_tx_limiting(void **state) {
    (void)state;  // silence unused warning
    size_t usize = uart_context_size();
    uint8_t ustore[usize];
    uart_t *pUART = (uart_t*)ustore;
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
            pUART, 
            &VTable, 
            115200, 
            rx_fifo, 
            sizeof(rx_fifo), 
            tx_fifo, 
            sizeof(tx_fifo)));

    const uint8_t msg[] = "TEST";
    size_t enq = uart_write(pUART, msg, sizeof(msg) - 1);
    // Check Tx FIFO enqueued at least some
    assert_true(enq >= 3);

    // Check first attempt only sends one byte
    uart_service_tx(pUART);
    assert_int_equal(1, CTX.tx_len);

    // Relax Tx bytes allowed and try again, checking that can send more now
    CTX.tx_bytes = 10;
    uart_service_tx(pUART);
    assert_true(CTX.tx_len >= 3);
}

//------------------------------------------------------------------------------
static void test_overflow_count(void **state) {
    (void)state;  // silence unused warning
    size_t usize = uart_context_size();
    uint8_t ustore[usize];
    uart_t *pUART = (uart_t*)ustore;
    uart_hw_vtable_t VTable;
    uart_stub_ctx_t CTX = {0};
    uint8_t tx_buf[4];
    uint8_t rx_buf[4];

    uart_hw_stub_create(&VTable, &CTX);
    assert_true(
        uart_init(
            pUART, 
            &VTable, 
            115200, 
            rx_buf, 
            sizeof(rx_buf), 
            tx_buf, 
            sizeof(tx_buf)));

    uart_rx_overflow_clear(pUART);
    // Fill Rx FIFO
    for (int i = 0; i < 4; i++) {
        uart_isr_rx_byte(pUART, (uint8_t)i);
    }
    // Force overflow
    uart_isr_rx_byte(pUART, 0x5A);
    // Verify condition
    assert_int_equal(1, uart_rx_overflow_count(pUART));
}

//------------------------------------------------------------------------------
static void test_echo_chunk_config(void **state) {
    (void)state;  // silence unused warning
    size_t usize = uart_context_size();
    uint8_t ustore[usize];
    uart_t *pUART = (uart_t*)ustore;
    uart_hw_vtable_t VTable;
    uart_stub_ctx_t CTX = {0};
    uint8_t rx_src[100];
    uint8_t rx_fifo[128];
    uint8_t tx_fifo[128];
    uint8_t tx_out[128];

    // Fill source data
    for (int i = 0; i < 100; i++) {
        rx_src[i] = (uint8_t)i;
    }

    CTX.prx_src = rx_src;
    CTX.rx_len = sizeof(rx_src);
   
    CTX.ptx_buf = tx_out;
    CTX.tx_capacity = sizeof(tx_out);
    CTX.tx_bytes = 100;

    uart_hw_stub_create(&VTable, &CTX);
    assert_true(
        uart_init(
            pUART, 
            &VTable, 
            115200, 
            rx_fifo, 
            sizeof(rx_fifo), 
            tx_fifo, 
            sizeof(tx_fifo)));

    // Check small chunk size
    uart_set_echo_chunk_size(pUART, 16);

    // Simulate receiving data
    for (size_t i = 0; i < CTX.rx_len; i++) {
        uart_isr_rx_byte(pUART, rx_src[i]);
    }

    uart_echo_pump(pUART);
    uart_service_tx(pUART);

    assert_int_equal(100, CTX.tx_len);
    assert_memory_equal(rx_src, tx_out, 100);
}

//------------------------------------------------------------------------------
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_init_validation),
        cmocka_unit_test(test_init_validation_requires_full_vtable),
        cmocka_unit_test(test_echo_basic),
        cmocka_unit_test(test_tx_limiting),
        cmocka_unit_test(test_overflow_count),
        cmocka_unit_test(test_echo_chunk_config),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
