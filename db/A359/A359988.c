/*
 * A359988  Number of edge cuts in the 3 X n grid graph.
 * https://oeis.org/A359988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Formula: a(n) = 54*a(n-1) - 777*a(n-2) + 2390*a(n-3) - 1736*a(n-4) + 256*a(n-5)  for n>5
 *  Initial values: a(1)=3, a(2)=105, a(3)=3665, a(4)=123215, a(5)=4051679
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache  = NULL;
    static idx_t cache_len = 0;   /* number of computed indices (highest index + 1) */
    static size_t cache_cap = 0;  /* allocated capacity */

    /* Initialise cache with the first 5 terms */
    if (cache_len == 0) {
        cache_cap = (n < 5) ? (size_t)6 : (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        /* if realloc fails in the initial call we return 0 – caller will not
           rely on the value for an invalid build */
        if (!cache) return (seq_t)0;
        cache[0] = 0;                     /* unused sentinel */
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)105;
        cache[3] = (seq_t)3665;
        cache[4] = (seq_t)123215;
        cache[5] = (seq_t)4051679;
        cache_len = 6;
    }

    /* If requested index already known, return it */
    if (n < cache_len) return cache[n];

    /* Ensure capacity to store up to index n */
    size_t new_cap = (size_t)(n + 1);
    if (new_cap > cache_cap) {
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failed */
        cache_cap = new_cap;
    }

    /* Compute values from the last known index up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence using seq_t coefficients */
        seq_t val = ((seq_t)54) * cache[i - 1];
        val -= ((seq_t)777) * cache[i - 2];
        val += ((seq_t)2390) * cache[i - 3];
        val -= ((seq_t)1736) * cache[i - 4];
        val += ((seq_t)256)  * cache[i - 5];
        cache[i] = val;
    }

    cache_len = n + 1;
    return cache[n];
}
