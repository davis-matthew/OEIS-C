/*
 * A377314  a(n) = coefficient of the term that is independent of 2^(1/3) and
 *          2^(2/3) in the expansion of (1 + 2^(1/3) + 2^(2/3))^n.
 * https://oeis.org/A377314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) + a(n-3), with initial values a(0)=1,
 *            a(1)=1, a(2)=5
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;    /* memoized values */
    static size_t cache_cap = 0;   /* total allocated capacity */
    static size_t cache_len = 0;   /* number of values currently stored */

    /* Ensure at least three base values are available */
    if (cache == NULL) {
        cache_cap = 4;            /* start with a small buffer */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)5;
        cache_len = 3;
    }

    /* Expand the cache if necessary */
    if ((size_t)n >= cache_len) {
        /* Grow the buffer to at least n+1 */
        if ((size_t)n + 1 > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 4;
            while (new_cap <= (size_t)n + 1) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;   /* reallocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute new terms using the recurrence */
        for (size_t k = cache_len; k <= (size_t)n; ++k) {
            cache[k] = 3 * cache[k - 1] + 3 * cache[k - 2] + cache[k - 3];
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
