/*
 * A355300  a(0) = 0; for n >= 1, a(n) = a(A007088(n) mod n) + 1.
 * https://oeis.org/A355300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0; a(n) = a(A007088(n) mod n) + 1 for n >= 1 A007088(n)
 *            is the decimal integer whose digits are the binary
 *            representation of n. The function below uses recursive
 *            memoisation with a dynamic cache
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   _binary_decimal_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/_binary_decimal_mod.h"

/* Recursive evaluation of a(n) with memoisation. */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* array of computed values */
    static bool  *used    = NULL;   /* flag array for 'cache' validity */
    static size_t cache_cap = 0;    /* current capacity of the arrays */

    /* Grow the cache if necessary. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        bool *new_used   = (bool *)realloc(used, new_cap * sizeof(bool));
        if (!new_cache || !new_used) abort();  /* out of memory */
        cache = new_cache;
        used  = new_used;
        /* Initialise the new entries as unused. */
        for (size_t i = cache_cap; i < new_cap; ++i) used[i] = false;
        cache_cap = new_cap;
    }

    /* Return cached value if already computed. */
    if (used[n]) return cache[n];

    /* Base case. */
    if (n == 0) {
        cache[0] = (seq_t)0;
        used[0]  = true;
        return cache[0];
    }

    /* Recursive step: a(n) = a(f(n)) + 1, where f(n) = A007088(n) mod n. */
    idx_t f   = _binary_decimal_mod(n);          /* f < n */
    seq_t val = a(f) + (seq_t)1;

    cache[n] = val;
    used[n]  = true;
    return val;
}
