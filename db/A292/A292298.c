/*
 * A292298  Sum of values of vertices of type D at level n of the hyperbolic
 *          Pascal pyramid.
 * https://oeis.org/A292298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=24, a(5)=324, a(6)=3600
 *            a(n) = 18*a(n-1) - 99*a(n-2) + 226*a(n-3) - 224*a(n-4) +
 *            92*a(n-5) - 12*a(n-6) (n ≥ 7) This is the recurrence given in
 *            the OEIS entry A292298
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 21, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache holding already computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* number of elements currently stored */

    /* Initialise cache with the first seven terms. */
    if (cache == NULL) {
        cache_cap = 7;                        /* indices 0 … 6 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = 0;  cache[1] = 0;  cache[2] = 0;  cache[3] = 0;
        cache[4] = 24; cache[5] = 324; cache[6] = 3600;
    }

    /* Return directly if the term is already cached. */
    if (n < cache_cap) {
        return cache[n];
    }

    /* Compute terms from the first missing index up to n. */
    idx_t start = cache_cap;                /* first index to compute */

    /* Grow the cache to accommodate index n. */
    if (cache_cap <= n) {
        size_t new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
    }

    for (idx_t i = start; i <= n; ++i) {
        seq_t v  = 18 * cache[i - 1];
        v      -= 99 * cache[i - 2];
        v      += 226 * cache[i - 3];
        v      -= 224 * cache[i - 4];
        v      += 92  * cache[i - 5];
        v      -= 12  * cache[i - 6];
        cache[i] = v;
    }

    return cache[n];
}
