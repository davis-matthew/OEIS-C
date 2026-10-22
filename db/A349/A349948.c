/*
 * A349948  Tetrahedral-sided isosceles Heron triangle pairs.
 * https://oeis.org/A349948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2) + 8 for n >= 3, with a(1) = 0, a(2) =
 *            10. This function computes the terms of OEIS sequence A349948
 *            using an iterative recurrence and memoises the results to
 *            support arbitrary call order
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the defined offset (a(1) is the first term). */
    if (n == 0) return 0;          /* undefined; return 0 safely */

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialise cache with the first three terms if needed. */
    if (cache == NULL) {
        /* Allocate space for at least the first three indices. */
        cache_size = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL) exit(1);      /* allocation failure */
        cache[0] = (seq_t)0;             /* a(1) */
        cache[1] = (seq_t)10;            /* a(2) */
        cache[2] = (seq_t)48;            /* a(3) */
    }

    /* If n is within the cached range, simply return it. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Grow the cache to accommodate up to index n. */
    idx_t old_size = cache_size;
    cache_size = n;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_size);
    if (new_cache == NULL) exit(1);      /* allocation failure */
    cache = new_cache;

    /* Compute new terms iteratively. */
    for (idx_t i = old_size; i < cache_size; ++i) {
        /* i is zero–based: cache[i] corresponds to a(i+1). */
        /* Compute a(i+1) = 4*a(i) - a(i-1) + 8. */
        seq_t term = cache[i - 1] << 2;      /* 4 * a(i)  */
        term += (seq_t)8;                   /* + 8       */
        term -= cache[i - 2];                /* - a(i-1)  */
        cache[i] = term;
    }

    return cache[n - 1];
}
