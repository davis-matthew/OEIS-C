/*
 * A291024  p-INVERT of (1,1,1,1,1,...), where p(S) = (1 - 2 S^2)^2.
 * https://oeis.org/A291024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 4*a(n-3) - a(n-4) initial values:
 *            a(0)=0, a(1)=4, a(2)=8, a(3)=24, a(4)=64 (p-INVERT of
 *            (1,1,1,1,1,...) with p(S) = (1-2S²)²)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoised table for previously computed terms */
static seq_t *cache = NULL;     /* array of computed a(n) */
static size_t cache_size = 0;   /* number of entries stored */

/* initial values a(0)..a(4) */
static const seq_t init[5] = {
    (seq_t)0,
    (seq_t)4,
    (seq_t)8,
    (seq_t)24,
    (seq_t)64
};

/* compute a(n) */
seq_t a(idx_t n)
{
    /* Ensure we have room for indices 0..n */
    if (n + 1 > cache_size) {
        /* Resize cache to new size */
        size_t new_size = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        /* Initialise from the first ten indices if needed */
        if (cache_size < new_size) {
            /* Fill missing initial values */
            size_t i;
            for (i = cache_size; i < 5 && i < new_size; ++i)
                cache[i] = init[i];

            /* Compute terms beyond the initial block */
            for (; i < new_size; ++i) {
                seq_t a1 = cache[i-1];
                seq_t a2 = cache[i-2];
                seq_t a3 = cache[i-3];
                seq_t a4 = cache[i-4];
                cache[i] = ((seq_t)4 * a1) - ((seq_t)2 * a2)
                          - ((seq_t)4 * a3) - a4;
            }
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
