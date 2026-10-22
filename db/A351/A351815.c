/*
 * A351815  G.f. A(x) satisfies A(x) = 1 + x * A(x/(1 - x)^5) / (1 - x).
 * https://oeis.org/A351815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   binomial, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1;
   a(n) = Σ_{k=0}^{n-1} binomial(n+4*k-1, n-k-1) * a(k)  */
#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity (number of entries) */
    static size_t computed = 0;    /* index of first term not yet computed */

    /* Initialise cache on first call */
    if (cache == NULL) {
        ensure_cache(1, &cache, &cache_cap);   /* a[0] only */
        cache[0] = (seq_t)1;
        computed = 1;                           /* next to compute is a[1] */
    }

    if ((idx_t)n < computed)
        return cache[n];

    /* Make sure the cache can hold up to a[n] */
    ensure_cache((idx_t)(n + 1), &cache, &cache_cap);

    /* Compute missing terms iteratively */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < i; ++k) {
            idx_t a1 = i + 4 * k - 1;
            idx_t b1 = i - k - 1;
            seq_t term = binomial(a1, b1) * cache[k];
            sum += term;
        }
        cache[i] = sum;
    }

    computed = n + 1;
    return cache[n];
}
