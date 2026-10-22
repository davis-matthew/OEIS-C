/*
 * A302101  G.f. A(x) satisfies: A(x) = 1 + x * (A(x) + 4*x*A'(x)) / (A(x) +
 *          x*A'(x)).
 * https://oeis.org/A302101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 1 For n ≥ 2: a(n) = (4n-3)·a(n-1) –
 *            Σ_{k=1}^{n-1} (k+1)·a(k)·a(n-k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

/* memoised array of already computed terms */
static seq_t *cache = NULL;     /* array of coefficients */
static size_t cache_cap = 0;    /* allocated size of the array          */
static size_t computed   = 0;   /* number of terms that are already set  */

/* compute a(n) -------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* make sure the array is large enough */
    if (cache_cap <= (size_t)n) {
        ensure_cache(n, &cache, &cache_cap);
    }

    /* initialise a(0) on first use */
    if (computed == 0) {
        cache[0] = (seq_t)1;
        computed = 1;
    }

    /* if we already have the value, just return it */
    if (n < computed) {
        return cache[n];
    }

    /* a(1) needs to be inserted, but only if n ≥ 1 */
    if (computed == 1 && n >= 1) {
        cache[1] = (seq_t)1;
        computed = 2;
    }

    /* compute successive terms up to n */
    for (idx_t i = (idx_t)computed; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k < i; ++k) {
            seq_t term = cache[k];
            term = term * (seq_t)(k + 1);        /* multiply by (k+1)   */
            term = term * cache[i - k];          /* multiply by a_{n-k}  */
            sum += term;                         /* accumulate          */
        }
        seq_t d = ((seq_t)(4 * (seq_t)i - 3)) * cache[i - 1];
        cache[i] = d - sum;
    }

    computed = (size_t)n + 1;
    return cache[(size_t)n];
}
