// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.
//------------------------------------------------------------------------------
//
// This module defines the functionality to install and initialize the STM32H5 
// UART hardware
//
//------------------------------------------------------------------------------

#include "uart_hw.h"

//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------

#define GPIO_REG(base, offset)   REG32((uintptr_t)(base) + (offset))
#define USART_REG(base, offset)  REG32((uintptr_t)(base) + (offset))

#define GPIO_MODE_MASK(pin)      (0x3u << ((pin) * 2u))
#define GPIO_AF_MASK(pin)        (0xFu << (((pin) % 8u) * 4u))
#define GPIO_AFR_OFFSET(pin)     (((pin) < 8u) ? GPIO_AFRL_OFFSET : GPIO_AFRH_OFFSET)

//------------------------------------------------------------------------------
// Function Definitions
//------------------------------------------------------------------------------
// Helper to set GPIO alternate function
static inline void gpio_set_af(uintptr_t gpio_base, uint32_t pin, uint32_t af) {
    uint32_t af_shift = (pin % 8u) * 4u;
    uint32_t mode_shift = pin * 2u;

    // Set GPIO Mode: 10 = Alternate Function
    GPIO_REG(gpio_base, GPIO_MODER_OFFSET) =
        (GPIO_REG(gpio_base, GPIO_MODER_OFFSET) & ~GPIO_MODE_MASK(pin)) |
        (GPIO_MODER_AF << mode_shift);

    // Set Alternate Function: AFRL for 0..7, AFRH for 8..15.
    GPIO_REG(gpio_base, GPIO_AFR_OFFSET(pin)) =
        (GPIO_REG(gpio_base, GPIO_AFR_OFFSET(pin)) & ~GPIO_AF_MASK(pin)) |
        ((af & 0xFu) << af_shift);

    // Set high speed, pull-up, push-pull.
    GPIO_REG(gpio_base, GPIO_OSPEEDR_OFFSET) =
        (GPIO_REG(gpio_base, GPIO_OSPEEDR_OFFSET) & ~GPIO_MODE_MASK(pin)) |
        (GPIO_OSPEED_HI << mode_shift);
    GPIO_REG(gpio_base, GPIO_PUPDR_OFFSET) =
        (GPIO_REG(gpio_base, GPIO_PUPDR_OFFSET) & ~GPIO_MODE_MASK(pin)) |
        (GPIO_PUPDR_PU << mode_shift);
    GPIO_REG(gpio_base, GPIO_OTYPER_OFFSET) &= ~(1u << pin);
}

//------------------------------------------------------------------------------
static bool hw_init(uint32_t baud) {
    if (baud == 0u) {
        return false;
    }

    UART_HW_EnableClocks();
    gpio_set_af(UART_HW_TX_GPIO, UART_HW_TX_PIN, UART_HW_AF_NUM);
    gpio_set_af(UART_HW_RX_GPIO, UART_HW_RX_PIN, UART_HW_AF_NUM);

    // Disable -> Configure 8N1 -> Enable
    USART_REG(UART_HW_USART, USART_CR1_OFFSET) &= ~USART_CR1_UE;
    USART_REG(UART_HW_USART, USART_CR1_OFFSET) = 0u;
    USART_REG(UART_HW_USART, USART_CR2_OFFSET) = 0u;
    USART_REG(UART_HW_USART, USART_CR3_OFFSET) = 0u;

    // Oversample by 16
    USART_REG(UART_HW_USART, USART_BRR_OFFSET) =
        (UART_HW_USART_CLK_HZ + (baud / 2u)) / baud;
    USART_REG(UART_HW_USART, USART_CR1_OFFSET) =
        USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;

    return true;
}

//------------------------------------------------------------------------------
static bool hw_tx_ready(void) {
    return (USART_REG(UART_HW_USART, USART_ISR_OFFSET) &
        USART_ISR_TXE_TXFNF) != 0u;
}

//------------------------------------------------------------------------------
static void hw_tx_write(uint8_t byte) {
    USART_REG(UART_HW_USART, USART_TDR_OFFSET) = byte;
}

//------------------------------------------------------------------------------
static bool hw_rx_available(void) {
    return (USART_REG(UART_HW_USART, USART_ISR_OFFSET) &
        USART_ISR_RXNE_RXFNE) != 0u;
}

//------------------------------------------------------------------------------
static uint8_t hw_rx_read(void) {
    return (uint8_t)USART_REG(UART_HW_USART, USART_RDR_OFFSET);
}

//------------------------------------------------------------------------------
void uart_hw_install(uart_hw_vtable_t *pv) {
    pv->hw_init = hw_init;
    pv->hw_tx_ready = hw_tx_ready;
    pv->hw_tx_write = hw_tx_write;
    pv->hw_rx_available = hw_rx_available;
    pv->hw_rx_read = hw_rx_read;
}

bool uart_hw_reinit(uint32_t baud) {
    return hw_init(baud);
}
