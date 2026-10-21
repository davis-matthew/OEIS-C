/*
 * A276535  a(n) = a(n-1) * a(n-6) * (a(n-2) * a(n-5) * (a(n-3) * a(n-4) + 1)
 *          + 1) / a(n-7), with a(0) = a(1) = a(2) = a(3) = a(4) = a(5) = a(6)
 *          = 1.
 * https://oeis.org/A276535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) * a(n-6) * (a(n-2) * a(n-5) * (a(n-3) * a(n-4) +
 *            1) + 1) / a(n-7) with a(0) = a(1) = a(2) = a(3) = a(4) = a(5) =
 *            a(6) = 1
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 11, u64 n <= 12, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;       /* allocated capacity (in entries) */
    static idx_t cache_len = 6;        /* highest index computed so far */

    /* Initialise cache for the first call */
    if (cache == NULL) {
        /* Start with a small capacity and grow as needed */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* Out of memory */
        /* Set initial 7 terms to 1 */
        for (idx_t i = 0; i <= 6; ++i)
            cache[i] = (seq_t)1;
    }

    /* Ensure we have space for index n */
    if ((idx_t)(cache_cap) <= n) {
        /* Double the capacity until it exceeds n */
        size_t new_cap = cache_cap;
        while ((idx_t)(new_cap) <= n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* Out of memory */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if (n > cache_len) {
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t t1 = cache[i - 3] * cache[i - 4] + (seq_t)1;
            seq_t t2 = cache[i - 2] * cache[i - 5] * t1 + (seq_t)1;
            seq_t t3 = cache[i - 1] * cache[i - 6] * t2;
            cache[i] = t3 / cache[i - 7];
        }
        cache_len = n;
    }

    return cache[n];
}
