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
// Doxygen Brief
//------------------------------------------------------------------------------

/** @file uart_api.h
 *  @brief Portable UART API with an opaque handle for clients to use.
 */

//------------------------------------------------------------------------------
// Types
//------------------------------------------------------------------------------

// Hardware API
// Functions to be called by portable core
/** @brief Virtual function table for a run-time installed UART backend. */
typedef struct {
    /** @brief Initialize HW at a given baud rate. @return true on success. */
    bool (*hw_init)(uint32_t baud);
    /** @brief Whether Tx can accept another byte right now. */
    bool (*hw_tx_ready)(void);
    /** @brief Write the given byte to the Tx FIFO. */
    void (*hw_tx_write)(uint8_t byte);
    // Polling Rx design:
    // Read one byte if there a byte ready
    /** @brief Whether an Rx byte is available (polling mode only). */
    bool (*hw_rx_available)(void);
    /** @brief Read one byte from Rx (check if available first). */
    uint8_t (*hw_rx_read)(void);
} uart_hw_vtable_t;

/** @brief One-per-instance opaque handle */
typedef struct uart_t uart_t;
/** @brief Helper function to query uart_t size in bytes for one uart_t context's storage allocation. */
size_t uart_context_size(void);

//------------------------------------------------------------------------------
// Function Declarations
//------------------------------------------------------------------------------
// Portable core
/** @brief Initialize a UART instance.
 *  @param pu       Opaque context pointer (caller-owned storage).
 *  @param phw      Backend virtual function table (copied internally).
 *  @param baud     Baud rate.
 *  @param prx_buf  Rx FIFO storage pointer (caller-provided).
 *  @param rx_size  Rx FIFO size in bytes.
 *  @param ptx_buf  Tx FIFO storage pointer (caller-provided).
 *  @param tx_size  Tx FIFO size in bytes.
 *  @return true on success.
 */
bool uart_init(
    uart_t *pu, 
    const uart_hw_vtable_t *phw, 
    uint32_t baud, 
    void *prx_buf, 
    size_t rx_size, 
    void *ptx_buf, 
    size_t tx_size);

//------------------------------------------------------------------------------
/** @brief ISR Rx Variant: Push inbound byte into FIFO from ISR.
 *  @param pu    Opaque context pointer (caller-owned storage).
 *  @param byte  Byte to push into FIFO.
 *  @return void.
 */
 void uart_isr_rx_byte(uart_t *pu, uint8_t byte);

//------------------------------------------------------------------------------
/** @brief Move queued Tx bytes to HW; Context: Main Loop or TX Empty ISR.
 *  @param pu    Opaque context pointer (caller-owned storage).
 *  @return void.
 */
void uart_service_tx(uart_t *pu);

//------------------------------------------------------------------------------
/** @brief Write data to Tx FIFO; Context: Application APIs.
 *  @param pu     Opaque context pointer (caller-owned storage).
 *  @param pdata  Pointer to user data to write.
 *  @param len    Max length of data to write.
 *  @return Size in bytes of data written to Tx FIFO.
 */
size_t uart_write(uart_t *pu, const uint8_t *pdata, size_t len);
/** @brief Get data queued in Tx FIFO; Context: Application APIs.
 *  @param pu     Opaque context pointer (caller-owned storage).
 *  @return Size in bytes of data queued in Tx FIFO.
 */
size_t uart_tx_queued(const uart_t *pu);
/** @brief Check if Rx available, then read; Context: Polling Rx Variant.
 *  @param pu  Opaque context pointer (caller-owned storage).
 *  @return Size in bytes of Rx data available in FIFO for read.
 */
size_t uart_rx_available(const uart_t *pu);
/** @brief Get data queued in Tx FIFO; Context: Application APIs.
 *  @param pu      Opaque context pointer (caller-owned storage).
 *  @param pout    User-owned buffer into which to read maxlen bytes.
 *  @param maxlen  Maximum length to read in bytes.
 *  @return Size in bytes of data read from Rx FIFO.
 */
size_t uart_read(uart_t *pu, uint8_t *pout, size_t maxlen);

//------------------------------------------------------------------------------
/** @brief Echo helper: read data from Rx FIFO, write to Tx FIFO.
 *  @param pu      Opaque context pointer (caller-owned storage).
 *  @return void.
 */
void uart_echo_pump(uart_t *pu);

//------------------------------------------------------------------------------
// Overflow Diagnostics
/** @brief Get number of bytes received but not queued due to a full Rx FIFO.
 *  @param pu      Opaque context pointer (caller-owned storage).
 *  @return Number of bytes of current Rx FIFO overflow.
 */
uint32_t uart_rx_overflow_count(const uart_t *pu);
/** @brief Clear number of bytes received but not queued due to a full Rx FIFO.
 *  @param pu      Opaque context pointer (caller-owned storage).
 *  @return void.
 */
void uart_rx_overflow_clear(uart_t *pu);

//------------------------------------------------------------------------------
// Override Drain Chunk Size
/** @brief Tune the number of bytes to drain (echo) from Rx to Tx.
 *  @param pu                Opaque context pointer (caller-owned storage).
 *  @param chunk_size_bytes  Number of bytes to echo in one attempt.
 *  @return void.
 */
void uart_set_echo_chunk_size(uart_t *pu, size_t chunk_size_bytes);
/** @brief Query current number of bytes echoed from Rx to Tx per attempt.
 *  @param pu  Opaque context pointer (caller-owned storage).
 *  @return Current number of bytes to echo in one attempt.
 */
size_t uart_get_echo_chunk_size(const uart_t *pu);

#endif // INCLUDE_UART_API_H_