/*
 * A294800  Number of permutations of [n] avoiding {1324, 2341, 3421}.
 * https://oeis.org/A294800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 26*a(n-2) + 45*a(n-3) - 45*a(n-4) + 26*a(n-5)
 *            - 8*a(n-6) + a(n-7); Initial values: a(0)=1, a(1)=1, a(2)=2,
 *            a(3)=6, a(4)=21, a(5)=73, a(6)=238. Recurrence holds for all n >
 *            6
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Compute the n‑th term of OEIS A294800 (permutation counting). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* previously computed values   */
    static size_t cache_size = 0;     /* number of cached entries      */

    /* Initialise the cache with the first 7 terms if necessary. */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(7 * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)21;
        cache[5] = (seq_t)73;
        cache[6] = (seq_t)238;
        cache_size = 7;
    }

    /* If the value is already cached, return it immediately. */
    if ((size_t)n < cache_size) {
        return cache[(size_t)n];
    }

    /* Grow the cache to encompass the desired index. */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* Compute missing values using the linear recurrence. */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        cache[i] = (8  * cache[i-1]
                   - 26 * cache[i-2]
                   + 45 * cache[i-3]
                   - 45 * cache[i-4]
                   + 26 * cache[i-5]
                   - 8  * cache[i-6]
                   +   cache[i-7]);
    }

    cache_size = new_size;
    return cache[(size_t)n];
}
