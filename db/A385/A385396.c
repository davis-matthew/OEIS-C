/*
 * A385396  Numbers k such that 8 does not divide binomial(k, j) for any j in
 *          0..k.
 * https://oeis.org/A385396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-4) - 2*a(n-5) for n>8, with a(1)=0,
 *            a(2)=1, a(3)=2, a(4)=3, a(5)=4, a(6)=5, a(7)=6, a(8)=7
 * Range:     u8 n <= 28, u16 n <= 60, u32 n <= 124, u64 n <= 252,
 *            u128 n <= 508, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached values for previously computed indices. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialize cache with first 8 terms if necessary. */
    if (cache == NULL) {
        cache_size = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL) {
            exit(1); /* Allocation failed */
        }
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 3;
        cache[4] = 4;
        cache[5] = 5;
        cache[6] = 6;
        cache[7] = 7;
    }

    /* If the requested index is already cached, return it. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Extend the cache to accommodate index n. */
    idx_t new_size = n;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
    if (new_cache == NULL) {
        exit(1); /* Allocation failed */
    }
    cache = new_cache;
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* Compute using the recurrence:
           a(i) = a(i-1) + 2*a(i-4) - 2*a(i-5)   (for i > 8)
        */
        cache[i - 1] = cache[i - 2] + 2 * cache[i - 5] - 2 * cache[i - 6];
    }
    cache_size = new_size;
    return cache[n - 1];
}
