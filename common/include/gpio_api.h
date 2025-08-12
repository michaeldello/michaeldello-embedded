// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.
//------------------------------------------------------------------------------
#ifndef INCLUDE_GPIO_API_H_
#define INCLUDE_GPIO_API_H_
//------------------------------------------------------------------------------
//
// This header specifies a generic GPIO API that can be implemented for various
// deployment contexts, e.g., execution on a target hardware device, or unit 
// testing on a development host. In the latter case, the implementation will be
// a stub.
//
//------------------------------------------------------------------------------

#include <stdint.h>

//------------------------------------------------------------------------------
// API Object Definition
//------------------------------------------------------------------------------

// Define functor prototypes for the API
typedef void (*gpio_write_fn)(uint32_t pin, uint8_t value);
typedef void (*gpio_toggle_fn)(uint32_t pin);

// Organize the GPIO API into an object type
typedef struct
{
    gpio_write_fn  write;
    gpio_toggle_fn toggle;
}
gpio_api_t;

// Instantiate the GPIO API and defer assignment to its execution context
extern gpio_api_t gpio;

//------------------------------------------------------------------------------
// Function Declarations
//------------------------------------------------------------------------------

void gpio_init(void);

#endif // INCLUDE_GPIO_API_H_