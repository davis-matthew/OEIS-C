/*
 * A272855  Ramanujan's gamma-series.
 * https://oeis.org/A272855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 82*a(n-1) + 82*a(n-2) - a(n-3) a(0) = 12, a(1) = 1010,
 *            a(2) = 83802
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Extend the cache if needed */
    if (cache_len <= n)
    {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;

        /* Allocate a new array of the desired size */
        seq_t *new_cache = alloc_seq((size_t)new_len);

        /* Copy old values, if any, into the new array */
        for (idx_t i = 0; i < old_len; ++i)
            new_cache[i] = cache[i];

        /* Free the old cache */
        if (cache)
            free_seq(cache);

        cache     = new_cache;
        cache_len = new_len;

        /* Ensure the base values are set */
        if (cache_len > 0) cache[0] = (seq_t)12;
        if (cache_len > 1) cache[1] = (seq_t)1010;
        if (cache_len > 2) cache[2] = (seq_t)83802;

        /* Compute any new terms required by the recurrence */
        idx_t start = old_len;
        if (start < 3) start = 3;
        for (idx_t i = start; i <= n; ++i)
        {
            /* Use (seq_t)82 to avoid type mismatches for arbitrary‑precision types */
            cache[i] = cache[i-1] * (seq_t)82 + cache[i-2] * (seq_t)82 - cache[i-3];
        }
    }

    return cache[n];
}
