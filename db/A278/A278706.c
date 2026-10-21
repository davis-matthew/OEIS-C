/*
 * A278706  a(n) = a(n-1) + a(n-3) + a(n-5) - a(n-6), a(0) = a(1) = a(2) = 1,
 *          a(3) = 2, a(4) = 3, a(5) = 5.
 * https://oeis.org/A278706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 55, u64 n <= 109,
 *            u128 n <= 217, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A278706:
   a(n) = a(n-1) + a(n-3) + a(n-5) - a(n-6),
   a(0)=a(1)=a(2)=1, a(3)=2, a(4)=3, a(5)=5 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Quick return for the first six terms */
    static const seq_t init[6] = {1, 1, 1, 2, 3, 5};
    if (n <= 5) return init[n];

    /* Cache of already computed values.
       cache_sz holds the number of valid entries in cache
       (i.e., valid indices are 0 .. cache_sz-1).  */
    static seq_t *cache = NULL;
    static idx_t cache_sz = 0;

    /* If we have the value cached, return it immediately */
    if (n < cache_sz) return cache[n];

    /* Ensure that the cache is large enough and initialise it when needed */
    if (cache_sz == 0) {
        /* First allocation: at least 64 elements or the needed
           number of elements, whichever is larger. */
        idx_t new_cap = 64;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!cache) return 0;           /* Out of memory; caller must handle */
        /* Fill the known initial values */
        for (idx_t i = 0; i < 6; ++i) cache[i] = init[i];
        cache_sz = 6;
    } else {
        /* Existing cache: maybe enlarge it to accommodate the request */
        idx_t new_cap = cache_sz;
        if (new_cap <= n) {
            while (new_cap <= n) new_cap <<= 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!cache) return 0;       /* Out of memory */
        }
    }

    /* Compute any missing values up to index n */
    idx_t start = cache_sz;
    if (start < 6) start = 6;           /* Start from the first unknown */
    for (idx_t i = start; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-3] + cache[i-5] - cache[i-6];
    }
    cache_sz = n + 1;    /* All indices 0..n are now valid */

    return cache[n];
}
