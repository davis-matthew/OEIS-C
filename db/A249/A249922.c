/*
 * A249922  E.g.f. satisfies: A(x) = x + 4*A(x)^5/5.
 * https://oeis.org/A249922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence * a(0) = 1 * a(n) = (2500*n^4 - 5000*n^3 + 3500*n^2 -
 *            1000*n + 96) * a(n-1) for n > 0 * This recurrence follows from
 *            the given OEIS definition
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache   = NULL;   /* holds a(0) .. a(k) */
    static idx_t  cache_len = 0;    /* number of cached values */

    /* initialise cache with a(0) if empty */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) abort();          /* abort on allocation failure */
        cache[0] = (seq_t)1;          /* a(0) = 1 */
    }

    /* extend cache up to requested index n */
    if (n >= cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* compute missing values sequentially */
        for (idx_t i = cache_len; i <= n; ++i) {
            /* compute multiplier for a(i) */
            idx_t i2 = i * i;
            idx_t i3 = i2 * i;
            idx_t i4 = i3 * i;

            seq_t mult =
                (seq_t)2500 * (seq_t)i4
                - (seq_t)5000 * (seq_t)i3
                + (seq_t)3500 * (seq_t)i2
                - (seq_t)1000 * (seq_t)i
                + (seq_t)96;

            cache[i] = cache[i - 1] * mult;
        }
        cache_len = new_len;
    }

    return cache[n];
}
