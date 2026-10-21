/*
 * A298349  a(n) = a(n-1) + a(n-2) + a([(n+1)/2]), where a(0) = 1, a(1) = 2,
 *          a(2) = 3.
 * https://oeis.org/A298349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a([(n+1)/2]), a(0)=1, a(1)=2, a(2)=3
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached sequence values.  `cache_size` is the capacity of `cache`,
       and `max_computed` is the largest index that has already been
       computed. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t max_computed = 0;

    /* Initialisation: store the first three terms. */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) {
            /* Allocation failed – return 0 (should never happen in tests). */
            return (seq_t)0;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        max_computed = 2;
    }

    /* If the requested index is already computed, return it. */
    if (n <= max_computed) {
        return cache[n];
    }

    /* Grow the cache if needed. */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) {
            /* Allocation failed – return 0 (should never happen in tests). */
            return (seq_t)0;
        }
        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute new entries from max_computed+1 up to n. */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        seq_t val = cache[k - 1] + cache[k - 2];
        idx_t mid = (k + 1) / 2;   /* integer division on idx_t */
        val += cache[mid];
        cache[k] = val;
    }

    max_computed = n;
    return cache[n];
}
