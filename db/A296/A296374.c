/*
 * A296374  a(0) = 3; a(n) = a(n-1)*(a(n-1)^2 - 3*a(n-1) + 4)/2.
 * https://oeis.org/A296374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 3; a(n) = a(n-1)*(a(n-1)^2 - 3*a(n-1) + 4)/2;
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* compute a(n) for sequence A296374 */
seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;      /* array of terms */
    static size_t cache_n   = 0;     /* number of terms stored */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* grow the cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 2;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise a(0) if this is the first call */
    if (cache_n == 0) {
        cache[0] = (seq_t)3;
        cache_n = 1;
    }

    /* compute missing terms up to n */
    for (size_t i = cache_n; i <= (size_t)n; ++i) {
        seq_t prev = cache[i - 1];            /* a(i-1)          */
        seq_t t    = prev * prev;             /* a^2             */
        t          = t - 3 * prev;           /* a^2 - 3a        */
        t          = t + 4;                   /* a^2 - 3a + 4    */
        seq_t val   = prev * t;               /* prev * (a^2-3a+4) */
        val /= 2;                             /* divide by 2     */
        cache[i] = val;                       /* store a(i)      */
    }

    /* update number of computed terms */
    cache_n = (size_t)n + 1;

    return cache[(size_t)n];
}
