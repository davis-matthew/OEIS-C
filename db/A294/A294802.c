/*
 * A294802  Number of permutations of [n] avoiding {3412, 3421, 1324}.
 * https://oeis.org/A294802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 24*a(n-2) + 32*a(n-3) - 16*a(n-4) for n > 5
 *            Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=21, a(5)=73
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 24, u64 n <= 52, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed sequence values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise base terms up to a(5) */
    if (cache_size == 0) {
        cache_size = 6;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;  /* out of memory, result undefined */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)21;
        cache[5] = (seq_t)73;
    }

    /* if requested n is already in the cache, return it */
    if (n < cache_size) return cache[n];

    /* extend cache up to n */
    idx_t new_size = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) return 0;  /* out of memory, result undefined */
    cache = new_cache;

    /* compute missing values using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* recurrence only valid for i > 5 */
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        cache[i] = ( (seq_t)8 * a1 )
                 - ( (seq_t)24 * a2 )
                 + ( (seq_t)32 * a3 )
                 - ( (seq_t)16 * a4 );
    }

    cache_size = new_size;
    return cache[n];
}
