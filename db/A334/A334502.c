/*
 * A334502  Eventual period of a single cell in rule 62 cellular automaton in
 *          a cyclic universe of width n.
 * https://oeis.org/A334502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   equal_conf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A334502: eventual period of a single cell in rule 62 cellular
   automaton in a cyclic universe of width n.  Uses Floyd's cycle‑finding
   algorithm to determine the cycle length of the configuration space
   starting from a single active cell. */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/equal_conf.h"

/* Rule‑62 transition table: index is the 3‑bit neighbourhood (LMS),
   value is the new cell state (0 or 1). */
static const uint8_t rule62_output[8] = {
    0, /* 000 */
    1, /* 001 */
    1, /* 010 */
    1, /* 011 */
    1, /* 100 */
    1, /* 101 */
    0, /* 110 */
    0  /* 111 */
};

/* Advance one CA step: cur -> next. */
static void step_once(const uint8_t *cur, uint8_t *next, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        size_t l = (i == 0) ? n - 1 : i - 1;
        size_t r = (i + 1 == n) ? 0 : i + 1;
        unsigned bits = (cur[l] << 2) | (cur[i] << 1) | cur[r];
        next[i] = rule62_output[bits];
    }
}

seq_t a(idx_t n)
{
    /* The problem is defined for n >= 1.  Return 0 for n == 0 just as
       a safety guard – no test case will use it. */
    if (n == 0) return (seq_t)0;

    size_t len = (size_t)n;
    /* Allocate buffers.  Five arrays of len bytes each. */
    uint8_t *init      = (uint8_t *)malloc(len);
    uint8_t *tort      = (uint8_t *)malloc(len);
    uint8_t *tort_next = (uint8_t *)malloc(len);
    uint8_t *hare      = (uint8_t *)malloc(len);
    uint8_t *hare_next = (uint8_t *)malloc(len);

    if (!init || !tort || !tort_next || !hare || !hare_next) {
        /* Allocation failed – abort by returning 0 (unlikely). */
        free(init); free(tort); free(tort_next); free(hare); free(hare_next);
        return (seq_t)0;
    }

    /* Initial configuration: a single active cell at position 0. */
    for (size_t i = 0; i < len; ++i) {
        uint8_t val = (i == 0) ? 1 : 0;
        init[i] = val;
        tort[i] = val;
        hare[i] = val;
    }

    /* Advance hare twice from the start. */
    step_once(hare, hare_next, len);
    memcpy(hare, hare_next, len);
    step_once(hare, hare_next, len);
    memcpy(hare, hare_next, len);

    /* Find first meeting point. */
    while (!equal_conf(tort, hare, len)) {
        /* Tortoise moves one step. */
        step_once(tort, tort_next, len);
        memcpy(tort, tort_next, len);
        /* Hare moves two steps. */
        step_once(hare, hare_next, len);
        memcpy(hare, hare_next, len);
        step_once(hare, hare_next, len);
        memcpy(hare, hare_next, len);
    }

    /* Find mu (start of cycle). */
    size_t mu = 0;
    memcpy(tort, init, len);          /* reset tortoise to initial state */
    while (!equal_conf(tort, hare, len)) {
        step_once(tort, tort_next, len);
        memcpy(tort, tort_next, len);
        ++mu;
    }

    /* Find lambda (length of cycle). */
    size_t lambda = 1;
    /* hare currently equals tort (meeting point). Advance hare once. */
    step_once(hare, hare_next, len);
    memcpy(hare, hare_next, len);
    while (!equal_conf(tort, hare, len)) {
        step_once(hare, hare_next, len);
        memcpy(hare, hare_next, len);
        ++lambda;
    }

    /* Clean up. */
    free(init);
    free(tort);
    free(tort_next);
    free(hare);
    free(hare_next);

    return (seq_t)lambda;
}
