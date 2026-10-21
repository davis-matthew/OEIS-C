/*
 * A241522  The number of P-positions in the game of Nim with up to 4 piles,
 *          allowing for piles of zero, such that the number of objects in
 *          each pile does not exceed n.
 * https://oeis.org/A241522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(3*b) + 6*c^2*2^b + a(c-1) where b = floor(log₂(n)), c
 *            = n + 1 - 2^b a(0) = 1, a(2^k-1) = 2^(3*k) (these are covered by
 *            the recurrence)
 * Range:     u8 n <= 5, u16 n <= 43, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Utility to compute 2^exp in seq_t */
#include "../utils/pow2.h"

/* Recursive, memoised calculation of a(n) */
static seq_t rec(idx_t n)
{
    /* static cache arrays */
    static seq_t *cache = NULL;
    static unsigned char *seen = NULL;   /* 0 = not computed, 1 = computed */
    static size_t cache_size = 0;

    /* grow cache if necessary */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        /* realloc must be cast in C++ */
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        seen  = (unsigned char *)realloc(seen,  new_size * sizeof(unsigned char));
        if (!cache || !seen) {
            /* allocation failed – return 0 (should never happen for test data) */
            return (seq_t)0;
        }
        /* initialise new entries */
        for (size_t i = cache_size; i < new_size; ++i)
            seen[i] = 0;
        cache_size = new_size;
    }

    if (seen[n])
        return cache[n];

    seq_t result;
    if (n == 0)
        result = (seq_t)1;
    else {
        /* compute b = floor(log2(n)) */
        unsigned b = 0;
        idx_t temp = n;
        while (temp > 1) {
            temp >>= 1;
            ++b;
        }

        idx_t pow2b = (idx_t)1 << b;          /* 2^b as unsigned index */
        idx_t c = n + 1 - pow2b;

        seq_t term1 = pow2(3*b);              /* 2^(3*b) */
        seq_t term2 = (seq_t)c;               /* start with c */
        term2 *= (seq_t)c;                    /* c^2 */
        term2 *= (seq_t)6;                    /* 6*c^2 */
        term2 *= pow2(b);                     /* *2^b */

        seq_t sub = (seq_t)0;
        if (c > 0)
            sub = rec(c - 1);                 /* a(c-1) */

        result = term1 + term2 + sub;
    }

    cache[n] = result;
    seen[n]  = 1;
    return result;
}

/* External entry point required by the build system */
seq_t a(idx_t n)
{
    return rec(n);
}
