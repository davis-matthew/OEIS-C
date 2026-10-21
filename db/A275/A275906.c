/*
 * A275906  Expansion of (1+x+x^2) / (1-4*x-4*x^2-x^3).
 * https://oeis.org/A275906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1)+4*a(n-2)+a(n-3), a(0)=1, a(1)=5, a(2)=25
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cached_len = 0;
    size_t idx = (size_t)n;

    /* Initialize cache with the first three terms */
    if (cached_len == 0) {
        cached_len = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * cached_len);
        if (!cache) return (seq_t)0;          /* allocation failed */
        cache[0] = 1;
        cache[1] = 5;
        cache[2] = 25;
    }

    /* Return the value if already computed */
    if (idx < cached_len) {
        return cache[idx];
    }

    /* Extend cache up to required index */
    if (idx + 1 > cached_len) {
        size_t new_len = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;

        /* Compute new terms using the recurrence */
        for (size_t i = cached_len; i < new_len; ++i) {
            /* Compute 4*cache[i-1] using only additions */
            seq_t t1 = cache[i-1] + cache[i-1];
            seq_t t2 = t1 + t1;       /* t2 == 4*cache[i-1] */
            /* Compute 4*cache[i-2] */
            seq_t t3 = cache[i-2] + cache[i-2];
            seq_t t4 = t3 + t3;       /* t4 == 4*cache[i-2] */
            cache[i] = t2 + t4 + cache[i-3];
        }
        cached_len = new_len;
    }

    return cache[idx];
}
