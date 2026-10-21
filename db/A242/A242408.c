/*
 * A242408  Numbers such that in ternary representation at least one pair of
 *          adjacent digits has a sum greater than 2.
 * https://oeis.org/A242408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 176, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_adj_sum_gt2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A242408: numbers whose ternary representation contains
   at least one adjacent pair of digits with a sum greater than 2.
   The function builds the sequence incrementally by testing successive
   integers; it does not rely on a closed formula. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_adj_sum_gt2.h"

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;      /* current capacity of cache */
    static idx_t found = 0;          /* number of terms stored in cache */
    static seq_t next_test = (seq_t)1; /* next integer to test */

    /* Ensure that the cache has enough space for at least n terms. */
    if (cache_cap < n) {
        idx_t new_cap = n * 2;
        if (new_cap < n)
            new_cap = n;            /* handle overflow of multiplication */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have found n of them. */
    while (found < n) {
        if (has_adj_sum_gt2(next_test)) {
            cache[found] = next_test;
            ++found;
        }
        ++next_test;
    }

    return cache[n - 1];   /* offset 1: a(1) is first term in cache */
}
