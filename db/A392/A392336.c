/*
 * A392336  Number of maximal matchings in the n-necklace graph.
 * https://oeis.org/A392336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 8*a(n-2) + 4*a(n-3) with a(1) = 3, a(2) = 25,
 *            a(3) = 111 (valid for all n >= 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;     /* number of entries currently filled (>= 1) */
    static idx_t cache_capacity = 0; /* allocated length of cache */

    if (n < 1)         /* the sequence is defined only for n>=1 */
        return (seq_t)0;

    /* Ensure capacity for index n */
    if (n + 1 > cache_capacity) {
        idx_t new_cap = cache_capacity ? cache_capacity * 2 : 8;
        while (new_cap < n + 1)
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_capacity = new_cap;
    }

    /* Initialise first terms if the cache is empty */
    if (cache_size == 0) {
        cache[0] = (seq_t)0;         /* unused placeholder */
        cache[1] = (seq_t)3;         /* a(1) */
        cache[2] = (seq_t)25;        /* a(2) */
        cache[3] = (seq_t)111;       /* a(3) */
        cache_size = 4;
    }

    /* Compute successive terms up to n if needed */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t term = 3 * cache[i-1] + 8 * cache[i-2] + 4 * cache[i-3];
        cache[i] = term;
    }
    if (n + 1 > cache_size)
        cache_size = n + 1;

    return cache[n];
}
