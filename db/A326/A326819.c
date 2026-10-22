/*
 * A326819  Irregular triangle read by rows; for n >= 0, the n-th row
 *          corresponds to the elements of the set {(n-k) AND k, k = 0..n}, in
 *          ascending order (where AND denotes the bitwise AND operator).
 * https://oeis.org/A326819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, look, base
 * Formula:   Exact enumeration of the rows of the triangle defined by (n‑k) &
 *            k, distinct values sorted in ascending order. a(0) = 0, a(1) =
 *            0, a(2) = 0, a(3) = 1, …
 * Range:     u8 n <= 9850, u16 n <= 9851, u32 n <= 9851, u64 n <= 9851,
 *            u128 n <= 9851, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* stored sequence values */
    static size_t cache_cap  = 0;      /* allocated capacity for cache */
    static size_t cache_cnt  = 0;      /* number of values stored so far */
    static idx_t  last_row   = (idx_t)-1; /* index of last processed row */

    /* Make sure the cache has room for the requested index */
    if (cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap <= n) new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) abort();
        cache_cap = new_cap;
    }

    /* Generate rows until the cache contains at least n+1 terms */
    while (cache_cnt <= n) {
        idx_t r = last_row + 1;                     /* next row number (uint64_t wrap) */

        /* Boolean array to mark which AND values appear in this row.
           The maximum possible value is r itself, so array size is r+1. */
        bool *seen = (bool *)calloc((size_t)r + 1, sizeof(bool));
        if (!seen) abort();

        /* Mark all values (r-k) & k for k = 0..r */
        for (idx_t k = 0; k <= r; ++k) {
            idx_t val = ((r - k) & k);
            seen[(size_t)val] = true;
        }

        /* Append the unique values in ascending order to the cache */
        for (size_t v = 0; v <= (size_t)r; ++v) {
            if (seen[v]) {
                cache[cache_cnt++] = (seq_t)v;
                /* If the cache is full, grow it (unlikely but safe) */
                if (cache_cnt == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!cache) abort();
                }
            }
        }

        free(seen);
        last_row = r;
    }

    return cache[n];
}
