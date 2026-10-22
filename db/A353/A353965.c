/*
 * A353965  Number of tilings of a 3 X n rectangle using 2 X 2 and 1 X 1 tiles
 *          and right trominoes.
 * https://oeis.org/A353965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 9*a(n-2) - a(n-3) + 2*a(n-4) Initial values:
 *            a(0) = 1, a(1) = 1, a(2) = 13, a(3) = 47
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* static cache holds already computed values */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;   /* number of allocated slots */
    static idx_t computed_upto = 0; /* first index beyond the last computed */

    /* initialise cache with base values on first call */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure → zero */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)13;
        cache[3] = (seq_t)47;
        computed_upto = 4; /* have values 0..3 ready */
    }

    /* If requested term already computed, return it */
    if (n < computed_upto)
        return cache[n];

    /* Grow the cache so that it has enough space for n */
    if (n >= cache_size) {
        idx_t new_size = cache_size ? cache_size * 2 : 4;
        while (new_size <= n)
            new_size *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failure → zero */
        cache    = tmp;
        cache_size = new_size;
    }

    /* Compute all missing terms from the first missing index up to n */
    for (idx_t i = computed_upto; i <= n; ++i) {
        seq_t term = (seq_t)3 * cache[i - 1]
                   + (seq_t)9 * cache[i - 2]
                   -          cache[i - 3]
                   + (seq_t)2 * cache[i - 4];
        cache[i] = term;
    }

    /* Update the range of already computed values */
    computed_upto = n + 1;

    return cache[n];
}
