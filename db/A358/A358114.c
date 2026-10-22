/*
 * A358114  a(n) = [x^n] (16*x*(32*x - 3) + 1)^(-1/2).
 * https://oeis.org/A358114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (24*(2*n - 1)*a(n-1) - 512*(n - 1)*a(n-2)) / n Initial
 *            values: a(0) = 1, a(1) = 24
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The recursion uses 128‑bit intermediates to avoid overflow when
   seq_t is a 64‑bit unsigned integer.  This works for both the
   fixed–width and arbitrary‑precision builds because way the cast
   to __int128 is performed only on values that are known to fit. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of values currently stored */
    static size_t cache_cap = 0;  /* allocated capacity */

    /* Initialise cache for first call */
    if (cache_len == 0) {
        cache_cap = 8;                        /* small start */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 24;
        cache_len = 2;
    }

    /* Grow cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* term1 = 24 * (2*i - 1) * a[i-1] */
        __int128 term1 = (__int128)cache[i - 1] *
                         ( __int128)24 *
                         ( __int128)(2 * i - 1);

        /* term2 = 512 * (i - 1) * a[i-2] */
        __int128 term2 = (__int128)cache[i - 2] *
                         ( __int128)512 *
                         ( __int128)(i - 1);

        __int128 num = term1 - term2;

        cache[i] = (seq_t)(num / i);  /* division is exact */
    }

    cache_len = n + 1;    /* all values up to n are now stored */

    return cache[n];
}
