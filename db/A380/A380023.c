/*
 * A380023  a(n) = a(n-1) * a(n-2) * (1 - 2 / (n * (n-1))), with a(1) = 2,
 *          a(2) = 3.
 * https://oeis.org/A380023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) * a(n-2) * (1 - 2/(n*(n-1)))
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 8, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache   = NULL;
    static idx_t cache_n = 0;

    /* Initialise cache with first two terms */
    if (!cache) {
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 2;   /* a(1) */
        cache[1] = 3;   /* a(2) */
        cache_n  = 2;
    }

    /* Return already computed value */
    if (n == 0) return 0;            /* not used, but safe */
    if (n <= cache_n) return cache[n-1];

    /* Grow cache to needed size */
    seq_t *new_cache = (seq_t*)realloc(cache, n * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;

    /* Compute missing terms */
    for (idx_t i = cache_n + 1; i <= n; ++i) {
        seq_t a_prev  = cache[i-2];    /* a(i-1) */
        seq_t a_prev2 = cache[i-3];    /* a(i-2) */
        seq_t denom   = (seq_t)i * ((seq_t)i - 1);   /* n*(n-1) */
        seq_t num     = denom - 2;                  /* n*(n-1) - 2 */

        /* Compute a(i) = a_prev * a_prev2 / denom * num */
        seq_t prod = a_prev * a_prev2;
        prod = prod / denom;          /* exact division */
        cache[i-1] = prod * num;
    }

    cache_n = n;
    return cache[n-1];
}
