/*
 * A372447  a(n) = A000523(A372443(n)); One less than the binary length of the
 *          n-th iterate of 27 with Reduced Collatz-function R.
 * https://oeis.org/A372447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000523(A372443(n)) – one less than the binary length of
 *            the n-th reduced‑Collatz iterate of 27. a(0) is the first term,
 *            a(1) the second, and so on
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   next_odd_collatz_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_odd_collatz_uint64.h"

/* Compute floor(log2(x)).  x>0.  Uses only shifts and comparisons,
   compatible with all allowed integer operations. */
static inline unsigned bitlen_uint64(uint64_t x)
{
    unsigned bl = 0;
    while (x != 0) {
        x >>= 1;
        bl++;
    }
    return bl;                       /* number of bits */
}

seq_t a(idx_t n)
{
    /* Static cache of already computed a(0)…a(k).  All static state
       persists across calls, enabling non‑monotonic access patterns. */
    static seq_t *cache = NULL;        /* array of a(i) values */
    static idx_t cache_size = 0;       /* number of cached values */
    static uint64_t current = 27;      /* current odd iterate of 27 */
    static bool reached_fixed = false; /* true once we hit the fixed point 1 */

    /* Extend cache up to requested index n. */
    while (cache_size <= n) {
        /* Compute a(k) = bitlen(current) - 1. */
        unsigned bl = bitlen_uint64(current);
        seq_t val = (seq_t)(bl - 1);

        /* Grow cache array. */
        if (cache_size == 0)
            cache = (seq_t *)malloc(sizeof(seq_t));
        else
            cache = (seq_t *)realloc(cache, (cache_size + 1) * sizeof(seq_t));

        cache[cache_size] = val;
        cache_size++;

        /* If we have reached the fixed point 1, every further a(i)=0. */
        if (current == 1) {
            reached_fixed = true;
            break;
        }

        /* Prepare for next iterate. */
        current = next_odd_collatz_uint64(current);
    }

    /* If the requested index comes after the fixed point, a(n)=0. */
    if (reached_fixed && n >= cache_size)
        return 0;

    /* Otherwise return the cached value. */
    return cache[n];
}
