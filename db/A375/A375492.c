/*
 * A375492  Number of compositions of n into powers of two that each divide
 *          the sum of previous powers.
 * https://oeis.org/A375492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(p)*a(n-p) if n is not a power of two, a(n) = a(p)^2 + 1
 *            if n is a power of two. initial values: a(0) = 1, a(1) = 1 (p is
 *            the largest power of two < n)
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   // size_t
#include <stdlib.h>   // malloc, realloc, free

/* The build supplies seq_t and idx_t.
   We use a simple dynamic array with memoisation. */
seq_t a(idx_t n)
{
    /* static cache: array of a(0) .. a(current_size-1) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements stored */

    /* initialise cache for the first call */
    if (cache == NULL) {
        /* allocate space for indices 0 and 1 */
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL) return 0; /* out of memory – unlikely for small n */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_size = 2;
    }

    /* if already computed, return immediately */
    if (n < cache_size)
        return cache[n];

    /* grow the cache to hold indices up to n inclusive */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (new_cache == NULL) return 0; /* out of memory */
    cache = new_cache;
    idx_t old_size = cache_size;
    cache_size = n + 1;

    /* compute missing values */
    for (idx_t i = old_size; i <= n; ++i) {
        /* power of two test – i>=2 is guaranteed here */
        if ((i & (i - 1)) == 0) {
            seq_t half = cache[i >> 1];
            cache[i] = half * half + (seq_t)1;
        } else {
            /* find largest power of two less than i */
            idx_t p = 1;
            while ((p << 1) <= i)
                p <<= 1;
            cache[i] = cache[p] * cache[i - p];
        }
    }

    return cache[n];
}
