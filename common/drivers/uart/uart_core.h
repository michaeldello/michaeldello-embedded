// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.
//------------------------------------------------------------------------------
#ifndef INCLUDE_UART_CORE_H_
#define INCLUDE_UART_CORE_H_
//------------------------------------------------------------------------------
//
// This header specifies a portable UART core API.
//
//------------------------------------------------------------------------------

#include <stdint.h>
#include "uart_api.h"
#include "ringbuf.h"

//------------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------

// Max number of bytes to echo at a time, tune for best performance
// Notes:
//    A more sophisticated implementation could read 
//    this as a configuration parameter from flash
static const size_t UART_ECHO_DRAIN_CHUNK_BYTES = 32;

#endif // INCLUDE_UART_CORE_H_