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
 * Helpers:   enters_cycle_4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/enters_cycle_4.h"

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
        terms = (seq_t *)malloc(capacity * sizeof(seq_t));
        count = 0;
        next_test = 1;
    }

    /* Expand the sequence until we have at least n terms. */
    while (count < n) {
        if (enters_cycle_4(next_test)) {
            /* Grow the array if necessary. */
            if (count == capacity) {
                capacity <<= 1;
                terms = (seq_t *)realloc(terms, capacity * sizeof(seq_t));
            }
            terms[count++] = (seq_t)next_test;
        }
        ++next_test;
    }

    return terms[n - 1];
}
