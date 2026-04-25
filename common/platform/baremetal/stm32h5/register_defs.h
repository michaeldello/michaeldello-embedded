// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.
//------------------------------------------------------------------------------
#ifndef INCLUDE_REGISTER_DEFS_H_
#define INCLUDE_REGISTER_DEFS_H_
//------------------------------------------------------------------------------
//
// This header defines the minimal set of register layouts needed to support
// implemented applications on a STM32H5 MCU. SoC base addresses are defined at
// the board level (platform).
//
//------------------------------------------------------------------------------
#include <stdint.h>

//------------------------------------------------------------------------------
// Doxygen Brief
//------------------------------------------------------------------------------

/** @file platform_config.h
 *  @brief STM32H563 board configurations.
 */

//------------------------------------------------------------------------------
// Register Definitions
//------------------------------------------------------------------------------

// Generic 32-bit register convenience accessor
#define REG32(addr) (*(volatile uint32_t *)(uintptr_t)(addr))

#define GPIO_MODER_OFFSET     0x00u
#define GPIO_OTYPER_OFFSET    0x04u
#define GPIO_OSPEEDR_OFFSET   0x08u
#define GPIO_PUPDR_OFFSET     0x0Cu
#define GPIO_AFRL_OFFSET      0x20u
#define GPIO_AFRH_OFFSET      0x24u

// Pin configs
// MODER: 10b -> Alternate function
// OSPEEDR: 10b -> High speed
// PUPDR: 01b -> Pull-up
#define GPIO_MODER_AF         0x2u
#define GPIO_OSPEED_HI        0x2u
#define GPIO_PUPDR_PU         0x1u

#define USART_CR1_OFFSET      0x00u
#define USART_CR2_OFFSET      0x04u
#define USART_CR3_OFFSET      0x08u
#define USART_BRR_OFFSET      0x0Cu
#define USART_ISR_OFFSET      0x1Cu
#define USART_RDR_OFFSET      0x24u
#define USART_TDR_OFFSET      0x28u

#define USART_CR1_UE          (1u << 0)  // USART enable
#define USART_CR1_RE          (1u << 2)  // Receiver enable
#define USART_CR1_TE          (1u << 3)  // Transmitter enable

#define USART_ISR_RXNE_RXFNE  (1u << 5)  // RX not empty / RX FIFO not empty
#define USART_ISR_TXE_TXFNF   (1u << 7)  // TX empty / TX FIFO not full

#endif // INCLUDE_REGISTER_DEFS_H_