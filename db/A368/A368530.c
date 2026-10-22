/*
 * A368530  a(n) = Sum_{k=1..n} k^3 * 4^(n-k).
 * https://oeis.org/A368530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + n^3, a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements currently stored */

    /* Extend the cache if necessary */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;  /* fallback – should not happen in tests */

        /* Copy existing values */
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        /* Ensure a(0) is present */
        if (cache_len == 0)
            new_cache[0] = 0;

        /* Compute missing entries using the recurrence */
        idx_t start = (cache_len == 0) ? 1 : cache_len;
        for (idx_t i = start; i <= n; ++i) {
            seq_t k   = (seq_t)i;
            seq_t k3  = k * k * k;
            new_cache[i] = new_cache[i - 1] * 4 + k3;
        }

        /* Replace old cache */
        if (cache)
            free(cache);
        cache = new_cache;
        cache_len = new_len;
    }

    return cache[n];
}
