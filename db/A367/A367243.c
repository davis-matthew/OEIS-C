/*
 * A367243  a(n) is the number of n-digit numbers whose difference between the
 *          largest and smallest digits is equal to 2.
 * https://oeis.org/A367243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 6*a(n-1) - 11*a(n-2) + 6*a(n-3) for n > 3 initial values:
 *            a(1) = 0, a(2) = 15, a(3) = 91
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* allocated size (number of slots)   */
    static idx_t  cache_filled = 0; /* largest index for which a() is known */

    /* Handle n == 0 gracefully – offset is 1, behaviour below offset does not matter */
    if (n == 0) return 0;

    /* Ensure cache has at least 4 slots to store a(1)..a(3) */
    if (cache_size < 4) {
        cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;                         /* allocation failed */
        cache[0] = 0;                                 /* unused */
        cache[1] = (seq_t)0;       /* a(1) */
        cache[2] = (seq_t)15;      /* a(2) */
        cache[3] = (seq_t)91;      /* a(3) */
        cache_filled = 3;
    }

    /* Grow the cache if necessary */
    if ((idx_t)n >= (idx_t)cache_size) {
        size_t new_size = cache_size;
        while ((idx_t)n >= (idx_t)new_size)
            new_size <<= 1;           /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute values up to n if not already done */
    if (n > cache_filled) {
        for (idx_t k = cache_filled + 1; k <= n; ++k) {
            seq_t term1 = cache[k-1] * (seq_t)6;   /* 6*a(k-1) */
            seq_t term2 = cache[k-2] * (seq_t)11;  /* 11*a(k-2) */
            seq_t term3 = cache[k-3] * (seq_t)6;   /* 6*a(k-3) */
            seq_t sum   = term1 + term3;          /* 6*a(k-1) + 6*a(k-3) */
            cache[k] = sum - term2;               /* minus 11*a(k-2)   */
        }
        cache_filled = n;
    }

    return cache[n];
}
