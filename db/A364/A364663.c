/*
 * A364663  a(n+1) = a(|n-a(n)*a(n-1)|)+1; a(0) = 0.
 * https://oeis.org/A364663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = a(|n - a(n)*a(n-1)|) + 1; a(0) = 0
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence definition with initial value a(0) = 0 */

#include <stdlib.h>
#include <stdint.h>

/* Memoised cache for previously computed values */
static seq_t *cache = NULL;
static idx_t  cache_size = 0;   /* highest index +1 that is already filled */
static size_t cache_cap  = 0;   /* current allocation size in seq_t entries */

/* Ensure the cache can address indices up to n */
static void ensure_cache(idx_t n)
{
    idx_t needed = n + 1;
    if (cache_cap >= (size_t)needed)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_buf)
        exit(1);          /* abort on allocation failure */

    cache = new_buf;
    cache_cap = new_cap;

    /* initialise new entries to 0 (necessary only for seq_t types
       where uninitialised memory could affect computations) */
    for (size_t i = cache_size; i < new_cap; ++i)
        cache[i] = 0;
}

/* Compute a(n) using memoisation and the recursive formula. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Grow the cache to cover the requested index */
    if (cache_size < n + 1) {
        ensure_cache(n);
        /* Compute all missing terms up to n inclusively */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 0;
            } else if (i == 1) {
                /* a(1) = a(|0 - a(0)*a(-1)|) + 1 = a(0) + 1 = 1. */
                cache[1] = 1;
            } else {
                /* prod = a(i-1) * a(i-2) */
                seq_t prod = cache[i-1] * cache[i-2];

                /* diff = |(i-1) - prod| */
                seq_t diff_abs;
                seq_t idx_i_1 = (seq_t)(i-1);
                if (idx_i_1 >= prod)
                    diff_abs = idx_i_1 - prod;
                else
                    diff_abs = prod - idx_i_1;

                /* The recurrence guarantees the index is < i,
                   so it is safe to index into cache. */
                idx_t idx = (idx_t)diff_abs;
                cache[i] = cache[idx] + 1;
            }
        }
        cache_size = n + 1;
    }
    return cache[n];
}
