// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.
//------------------------------------------------------------------------------
#ifndef INCLUDE_PLATFORM_CONFIG_H_
#define INCLUDE_PLATFORM_CONFIG_H_
//------------------------------------------------------------------------------
//
// This header defines the STM32H563 board configuration details such as pins 
// and clocks.
//
//------------------------------------------------------------------------------

#include <stdint.h>
#include <stdbool.h>
#include "register_defs.h"

//------------------------------------------------------------------------------
// Doxygen Brief
//------------------------------------------------------------------------------

/** @file platform_config.h
 *  @brief STM32H563 board configurations.
 */

// Protect against global polutions:
//
// If no vendor header included, this file defines its own minimal constants so 
// it still builds
//
// If a vendor header is included in another build configuration, don’t redefine
// the same symbols

// -------- Base addresses --------

#ifndef RCC_BASE
#define RCC_BASE              0x44020C00u
#endif

#ifndef GPIOD_BASE
#define GPIOD_BASE            0x42020C00u
#endif

#ifndef USART3_BASE
#define USART3_BASE           0x40004800u
#endif

// -------- RCC clock-enable register addresses & bitmasks --------

#ifndef RCC_AHB2ENR_ADDR
#define RCC_AHB2ENR_ADDR      (RCC_BASE + 0x0000008Cu)
#endif
#ifndef RCC_AHB4ENR_ADDR
#define RCC_AHB4ENR_ADDR      (RCC_BASE + 0x00000094u)
#endif
#ifndef RCC_APB1LENR_ADDR
#define RCC_APB1LENR_ADDR     (RCC_BASE + 0x0000009Cu)
#endif

// Bit mask to enable GPIOD peripheral clock
#ifndef RCC_EN_GPIOD
#define RCC_EN_GPIOD          (1u << 3)
#endif

// Bit mask to enable USART3 peripheral clock
#ifndef RCC_EN_USART3
#define RCC_EN_USART3         (1u << 18)
#endif

// -------- UART pin mux (matching STM32H5xx routing) --------
#ifndef UART_TX_GPIO_BASE
#define UART_TX_GPIO_BASE     GPIOD_BASE
#endif
// PD8 by default
#ifndef UART_TX_PIN
#define UART_TX_PIN           8u
#endif
#ifndef UART_RX_GPIO_BASE
#define UART_RX_GPIO_BASE     GPIOD_BASE
#endif
// PD9 by default
#ifndef UART_RX_PIN
#define UART_RX_PIN           9u
#endif
// AF for USART on those pins
#ifndef UART_AF_NUM
#define UART_AF_NUM           7u
#endif

// -------- UART instance selection & clock --------
#ifndef UART_USART_BASE
#define UART_USART_BASE       USART3_BASE
#endif

#ifndef UART_USART_CLK_HZ
#define UART_USART_CLK_HZ     64000000u
#endif

/* -------- Small helpers -------- */
static inline void UART_EnableClocks(void) {
    REG32(RCC_AHB2ENR_ADDR)  |= RCC_EN_GPIOD;
    REG32(RCC_APB1LENR_ADDR) |= RCC_EN_USART3;
    (void)REG32(RCC_AHB2ENR_ADDR);
    (void)REG32(RCC_APB1LENR_ADDR);
}

//------------------------------------------------------------------------------
// App Configs
//------------------------------------------------------------------------------

///------------------------------------------------------------------------------
// Blinky App
//
// RCC and GPIO Register Values
#define RCC_AHB4ENR (*(volatile uint32_t*)0x580244E0)
#define GPIOB_MODER (*(volatile uint32_t*)0x58020400)
#define GPIOB_ODR   (*(volatile uint32_t*)0x58020414)
//
// GPIOB Location
#define GPIOB ((uint32_t*)0x58020400)

//------------------------------------------------------------------------------
// UART App
//
// Use ST-LINK VCP connected USART on NUCLEO-H563ZI
// USART3 on PD8 (TX) / PD9 (RX) with AF7 by default.
#ifndef UART_HW_USART
#define UART_HW_USART       UART_USART_BASE
#endif
//
// Enable peripheral and GPIO clocks for configured USART and pins
static inline void UART_HW_EnableClocks(void) {
    UART_EnableClocks();
}
//
#ifndef UART_HW_TX_GPIO
#define UART_HW_TX_GPIO     UART_TX_GPIO_BASE
#endif
#ifndef UART_HW_TX_PIN
#define UART_HW_TX_PIN      UART_TX_PIN
#endif
#ifndef UART_HW_RX_GPIO
#define UART_HW_RX_GPIO     UART_RX_GPIO_BASE
#endif
#ifndef UART_HW_RX_PIN
#define UART_HW_RX_PIN      UART_RX_PIN
#endif
#ifndef UART_HW_AF_NUM
#define UART_HW_AF_NUM      UART_AF_NUM
#endif
//
// Core clock feeding USART for BRR calculation
// TBD: Adjust if clocking from different bus
#ifndef UART_HW_USART_CLK_HZ
// TBD: 64MHz APB1, adjust per board
#define UART_HW_USART_CLK_HZ  64000000u
#endif

#endif // INCLUDE_PLATFORM_CONFIG_H_
