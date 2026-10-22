/*
 * A384673  Expansion of (1+x) / (1-2*x-5*x^2+2*x^3).
 * https://oeis.org/A384673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = 2*a(n-1) + 5*a(n-2) - 2*a(n-3), a(0)=1, a(1)=3, a(2)=11
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Coefficients of (1+x)/(1-2*x-5*x^2+2*x^3)) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache   = NULL;     /* array of a(0)..a(cache_len-1) */
    static size_t cache_cap = 0;      /* allocated array capacity */
    static size_t cache_len = 0;      /* number of values actually stored */

    /* Ensure the cache can hold indices up to n */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_cap) {
        size_t new_cap = needed;
        if (new_cap < 16) new_cap = 16;
        while (new_cap < needed) new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure – return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)3;
        } else if (i == 2) {
            cache[i] = (seq_t)11;
        } else {
            /* a(i) = 2*a(i-1) + 5*a(i-2) - 2*a(i-3) */
            cache[i] = ((seq_t)2 * cache[i - 1]) +
                       ((seq_t)5 * cache[i - 2]) -
                       ((seq_t)2 * cache[i - 3]);
        }
    }

    cache_len = (size_t)(n + 1);
    return cache[n];
}
