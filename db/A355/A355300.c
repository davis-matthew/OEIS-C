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
 * Validated: partial, pass, 93 terms through n = 92 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Compute A007088(n) modulo n.  The value is interpreted as a decimal
   number whose digits are the binary digits of n.  Only the remainder
   modulo n is required, so the computation stays small. */
static idx_t _binary_decimal_mod(idx_t n)
{
    /* Number of bits of n (n > 0). */
    idx_t bits = 0;
    idx_t tmp = n;
    while (tmp) {
        ++bits;
        tmp >>= 1;
    }

    idx_t rem = 0;
    for (idx_t pos = bits; pos > 0; --pos) {
        idx_t bit = (n >> (pos - 1)) & 1U;
        unsigned __int128 prod = (unsigned __int128)rem * 10 + bit;
        rem = (idx_t)(prod % n);
    }
    return rem;
}

/* Recursive evaluation of a(n) with memoisation. */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* array of computed values */
    static bool  *used    = NULL;   /* flag array for 'cache' validity */
    static size_t cache_cap = 0;    /* current capacity of the arrays */

    /* Grow the cache if necessary. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
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
