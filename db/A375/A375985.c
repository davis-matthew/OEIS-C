/*
 * A375985  Number of subsets of {1,2,...,n} such that no two elements differ
 *          by 1, 3, 4, or 5.
 * https://oeis.org/A375985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + a(n-6) + a(n-8) for n >= 8 Initial values:
 *            a(0)=1, a(1)=2, a(2)=3, a(3)=5, a(4)=7, a(5)=9, a(6)=11, a(7)=14
 * Range:     u8 n <= 16, u16 n <= 35, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n) {
    /* Memoisation cache */
    static seq_t *cache = NULL;      /* pointer to array of seq_t */
    static idx_t cache_cap = 0;      /* number of elements stored so far */

    /* Initial values for 0 <= i < 8 */
    static const seq_t init[8] = {1, 2, 3, 5, 7, 9, 11, 14};

    /* Helper to allocate or grow the cache */
    if (cache_cap == 0) {
        /* First call – establish a capacity that covers at least n */
        cache_cap = (n < 8 ? 8 : n + 1);
#ifdef __cplusplus
        cache = new seq_t[cache_cap];
#else
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
#endif
        if (!cache) return 0;  /* allocation failed – caller must check */

        /* Initialise known values */
        for (idx_t i = 0; i < cache_cap && i < 8; ++i)
            cache[i] = init[i];

        /* Compute remaining entries up to cache_cap-1 */
        for (idx_t i = 8; i < cache_cap; ++i)
            cache[i] = cache[i-1] + cache[i-6] + cache[i-8];
    }
    else if (n + 1 > cache_cap) {
        /* Need more space – grow the cache */
        idx_t new_cap = n + 1;
#ifdef __cplusplus
        seq_t *new_cache = new seq_t[new_cap];
        for (idx_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
        cache = new_cache;
#else
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failed */
        cache = new_cache;
#endif
        /* Initialise newly added positions */
        for (idx_t i = 8; i < new_cap; ++i) {
            /* If we started with n >= 8, some lower indices are already
               filled from the previous allocation. Only compute the new ones. */
            if (i >= cache_cap)
                cache[i] = cache[i-1] + cache[i-6] + cache[i-8];
        }
        /* For indices < 8 that might have been overwitten if cache_cap
           was initially less than 8, ensure they are correct. */
        for (idx_t i = 0; i < 8 && i < new_cap; ++i)
            cache[i] = init[i];
        cache_cap = new_cap;
    }

    return cache[n];
}
