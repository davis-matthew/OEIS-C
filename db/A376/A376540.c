/*
 * A376540  Natural numbers whose iterated squaring modulo 1000 eventually
 *          enters the 4-cycle 176, 976, 576, 776.
 * https://oeis.org/A376540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth natural number x such that iterated squaring modulo
 *            1000 eventually reaches the 4‑cycle 176 → 976 → 576 → 776.
 *            Computed by incremental search with memoisation
 * Range:     u8 none, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static bool enters_cycle_4(idx_t val)
{
    /* Check whether repeated squaring of val % 1000 reaches the required cycle. */
    seq_t y = (seq_t)val % 1000;
    for (int i = 0; i < 32; ++i) {
        /* Immediate attractors that do NOT belong to the desired cycle. */
        if (y == 0 || y == 1 || y == 376 || y == 625)
            return false;
        /* One of the desired cycle entries. */
        if (y == 176 || y == 976 || y == 576 || y == 776)
            return true;
        y = (y * y) % 1000;
    }
    /* Should never reach here for natural numbers, but keep safe. */
    return false;
}

seq_t a(idx_t n)
{
    /* Static storage for the sequence terms found so far. */
    static seq_t *terms = NULL;
    static idx_t   count = 0;      /* number of terms stored so far */
    static idx_t   capacity = 0;   /* current allocation size */
    static idx_t   next_test = 1;  /* next natural number to test */

    /* Initialise the dynamic array on first call. */
    if (terms == NULL) {
        capacity = 8;
        terms = mem_new(seq_t, capacity);
        count = 0;
        next_test = 1;
    }

    /* Expand the sequence until we have at least n terms. */
    while (count < n) {
        if (enters_cycle_4(next_test)) {
            /* Grow the array if necessary. */
            if (count == capacity) {
                capacity <<= 1;
                terms = mem_resize(seq_t, terms, capacity);
            }
            terms[count++] = (seq_t)next_test;
        }
        ++next_test;
    }

    return terms[n - 1];
}
