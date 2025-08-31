// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.
//------------------------------------------------------------------------------
//
// Define Ring Buffer unit tests
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
#include "ringbuf.h"

//------------------------------------------------------------------------------
// Test Definitions
//------------------------------------------------------------------------------
static void test_push_pop(void**) {
    uint8_t storage[4];
    ringbuf_t r;
    ringbuf_init(&r, storage, sizeof(storage));
    assert_true(ringbuf_push(&r, 'A'));
    assert_true(ringbuf_push(&r, 'B'));
    uint8_t byte = 0;
    assert_true(ringbuf_pop(&r, &byte));
    assert_int_equal('A', byte);
    assert_true(ringbuf_pop(&r, &byte));
    assert_int_equal('B', byte);
    assert_false(ringbuf_pop(&r, &byte));
}

//------------------------------------------------------------------------------
static void test_overflow(void**) {
    uint8_t storage[2];
    ringbuf_t r;
    ringbuf_init(&r, storage, sizeof(storage));
    assert_true(ringbuf_push(&r, 1));
    assert_true(ringbuf_push(&r, 2));
    // Should be full
    assert_false(ringbuf_push(&r, 3));
}

//------------------------------------------------------------------------------
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_push_pop),
        cmocka_unit_test(test_overflow),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}