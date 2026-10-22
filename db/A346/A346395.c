/*
 * A346395  Expansion of e.g.f. -log(1 - x) * exp(3*x).
 * https://oeis.org/A346395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+2)*a(n‑1) - 3*(n‑1)*a(n‑2) + 3^(n‑1), a(0) = 0, a(1) =
 *            1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* size_t */
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Cache of already computed values */
    static seq_t *cache          = NULL;
    static size_t cache_cap      = 0;
    static idx_t  cache_len      = 0;
    static seq_t  pow3           = 1;   /* will be set to 3 for the first term */

    /* First invocation: allocate a small array and initialise a(0) and a(1) */
    if (!cache) {
        cache_cap = 2;
        cache     = seq_alloc(cache_cap);
        cache[0]  = (seq_t)0;
        cache[1]  = (seq_t)1;
        cache_len = 2;
        pow3      = 3;  /* 3^(1) – ready for computing a(2) */
    }

    /* Ensure the cache is large enough for the requested index
       (doubling strategy keeps allocation O(log n)) */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Compute all missing terms up to n inclusive */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i >= 2) {
            /* Recurrence: a(i) = (i+2)*a(i-1) - 3*(i-1)*a(i-2) + 3^(i-1) */
            seq_t term = ((seq_t)(i + 2) * cache[i - 1]) -
                         ((seq_t)3 * (seq_t)(i - 1) * cache[i - 2]) +
                         pow3;
            cache[i] = term;
            pow3     = pow3 * 3;   /* prepare 3^i for the next iteration */
        }
    }
    cache_len = n + 1;
    return cache[n];
}
