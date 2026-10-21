/*
 * A291038  p-INVERT of (1,0,0,1,0,0,1,0,0,...), where p(S) = (1 - 2 S)^2.
 * https://oeis.org/A291038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + 2*a(n-3) - 4*a(n-4) - a(n-6) for
 *            n>=7 Initial values: a(0)=4, a(1)=12, a(2)=32, a(3)=84,
 *            a(4)=216, a(5)=544, a(6)=1348
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 50, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoised cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;          /* number of elements stored */

    /* initialise cache with the six known values */
    if (cache == NULL) {
        cache_size = 7;                   /* indices 0..6 inclusive */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return 0; /* out of memory – unlikely in tests */

        cache[0] = 4;   /* a(0) */
        cache[1] = 12;  /* a(1) */
        cache[2] = 32;  /* a(2) */
        cache[3] = 84;  /* a(3) */
        cache[4] = 216; /* a(4) */
        cache[5] = 544; /* a(5) */
        cache[6] = 1348;/* a(6) */
    }

    /* If the requested index is already cached, return it straight away */
    if (n < cache_size) {
        return cache[n];
    }

    /* Grow the cache to contain at least up to index n */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (new_cache == NULL) {
        /* If reallocation fails we cannot compute the requested term */
        return 0;
    }
    cache = new_cache;

    /* Compute the missing terms one by one, using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* Recurrence only applies for i >= 7, but the above loop starts
           at cache_size which is at least 7 if we are here.               */
        seq_t t1 = 4 * cache[i-1];
        seq_t t2 = 4 * cache[i-2];
        seq_t t3 = 2 * cache[i-3];
        seq_t t4 = 4 * cache[i-4];
        seq_t t6 = cache[i-6];
        cache[i] = t1 - t2 + t3 - t4 - t6;
    }

    cache_size = n + 1;  /* update the cache size after filling */
    return cache[n];
}
