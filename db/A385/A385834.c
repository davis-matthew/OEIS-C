/*
 * A385834  a(0) = 1; a(n) = Sum_{k=0..n-1} (1 + k^6) * a(k) * a(n-1-k).
 * https://oeis.org/A385834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} (1 + k^6) * a(k) * a(n-1-k)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc, free */

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* array of computed values */
    static size_t cache_cap = 0;    /* allocated capacity of cache   */
    static idx_t  cache_max  = 0;    /* largest index computed so far */

    /* Ensure cache[0] is initialized to 1 */
    if (cache_cap == 0) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        /* If allocation fails, return 0 (undefined behavior otherwise) */
        if (cache == NULL) return 0;
        cache[0] = (seq_t)1;
    }

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) return 0;        /* allocation failure */
        /* Zero-initialize newly allocated slots */
        for (size_t i = cache_cap; i < new_cap; ++i)
            new_cache[i] = (seq_t)0;
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Return cached value if already computed */
    if (n <= (idx_t)cache_max)
        return cache[n];

    /* Compute missing values up to n */
    for (idx_t m = cache_max + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = (idx_t)0; k < m; ++k) {
            /* w = 1 + k^6 */
            seq_t k_seq = (seq_t)k;
            seq_t k2   = k_seq * k_seq;   /* k^2 */
            seq_t k4   = k2 * k2;         /* k^4 */
            seq_t k6   = k4 * k2;         /* k^6 */
            seq_t w    = (seq_t)1 + k6;   /* 1 + k^6 */

            sum += w * cache[k] * cache[m - 1 - k];
        }
        cache[m] = sum;
    }
    cache_max = n;
    return cache[n];
}
