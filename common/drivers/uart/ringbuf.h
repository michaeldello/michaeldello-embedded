// Copyright (c) 2025 Michael Dello
//
// This software is provided under the MIT License.
// See LICENSE file for details.
//------------------------------------------------------------------------------
#ifndef INCLUDE_RING_BUF_H_
#define INCLUDE_RING_BUF_H_
//------------------------------------------------------------------------------
//
// This header specifies the API for a portable, testable ring buffer.
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Types
//------------------------------------------------------------------------------

typedef struct ringbuf_t {
    uint8_t *pbuf;
    size_t  capacity;
    size_t  head;
    size_t  tail;
    size_t  count;
} ringbuf_t;

//------------------------------------------------------------------------------
// Inline Function Definitions
//------------------------------------------------------------------------------
static inline void ringbuf_init(ringbuf_t *pr, void *pstorage, size_t size) {
    pr->pbuf = (uint8_t*)pstorage;
    pr->capacity = size;
    pr->head = 0;
    pr->tail = 0;
    pr->count = 0;
}

//------------------------------------------------------------------------------
static inline size_t ringbuf_available(const ringbuf_t *pr) {
    return pr->count;
}

//------------------------------------------------------------------------------
static inline size_t ringbuf_space(const ringbuf_t *pr) {
    return pr->capacity - pr->count;
}

//------------------------------------------------------------------------------
static inline bool ringbuf_push(const ringbuf_t *pr, uint8_t byte) {
    if (pr->count == pr->capacity) return false;
    pr->pbuf[pr->head] = byte;
    pr->head = (pr->head + 1) % pr->capacity;
    pr->count++;
    return true;
}

//------------------------------------------------------------------------------
static inline bool ringbuf_pop(const ringbuf_t *pr, uint8_t *pout) {
    if (pr->count == 0) return false;
    pout = pr->pbuf[pr->tail];
    pr->tail = (pr->tail + 1) % pr->capacity;
    pr->count--;
    return true;
}

#endif // INCLUDE_RING_BUF_H_