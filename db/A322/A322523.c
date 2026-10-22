/*
 * A322523  a(n) is the least nonnegative integer k for which there does not
 *          exist i < j with i+j=n and a(i)=a(j)=k.
 * https://oeis.org/A322523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS sequence A322523
 * a(n) = the least nonnegative integer k such that there does not exist indices
 * i < j with i + j = n and a(i) = a(j) = k.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;      /* array indexed from 1 */
    static idx_t cache_cap = 0;      /* allocated capacity (inclusive of index 0) */
    static idx_t cache_size = 0;     /* number of terms already computed */
    static seq_t max_value = 0;      /* current maximum value in cache */

    /* Ensure cache is large enough to hold index n */
    if (n > cache_cap) {
        idx_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
        while (new_cap < n) new_cap <<= 1;
        queue_refresh:
        seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!new_cache) {
            /* Out of memory; keep old cache and return 0 (should not happen for 10000) */
            return 0;
        }
        if (!cache) {
            /* Initial allocation, zero out the array */
            for (idx_t i = 0; i <= new_cap; ++i) new_cache[i] = 0;
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute terms up to n */
    for (idx_t m = cache_size + 1; m <= n; ++m) {
        /* Mark for each k whether there exists a pair (i,j) with i<j, i+j=m, and a(i)=a(j)=k */
        size_t used_size = (size_t)max_value + 2;  /* we need indices 0..max_value+1 */
        bool *used = (bool *)calloc(used_size, sizeof(bool));
        if (!used) return 0;  /* allocation failed */

        idx_t limit = (m - 1) / 2;
        for (idx_t i = 1; i <= limit; ++i) {
            seq_t left  = cache[i];
            seq_t right = cache[m - i];
            if (left == right) {
                used[(size_t)left] = true;
            }
        }

        /* Find smallest k not marked */
        seq_t val = 0;
        while (val < (seq_t)used_size && used[(size_t)val]) ++val;

        cache[m] = val;
        if (val > max_value) max_value = val;
        free(used);
    }

    cache_size = n;
    return cache[n];
}
