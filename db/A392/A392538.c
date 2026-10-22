/*
 * A392538  G.f. A(x) satisfies: A(x) = 1 + x * A(x/(1 - 2*x)^2) / (1 -
 *          2*x)^2.
 * https://oeis.org/A392538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = ∑_{k=0}^{n-1} binomial(n+k, n-k-1) * 2^(n-k-1)
 *            * a(k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   binomial, pow2, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/seq_realloc.h"

/* memoised cache of the sequence values */
static seq_t *cache      = NULL;   /* array of computed terms        */
static idx_t max_computed   = 0;   /* largest index already computed*/
static size_t  cache_cap    = 0;   /* current allocated capacity     */

/* grow the cache so that indices 0..n are valid */
static void ensure_capacity(idx_t n)
{
    if (cache_cap > n) return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    if (!tmp) exit(EXIT_FAILURE);        /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* main entry point for the OEIS sequence A392538 */
seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL) {
        ensure_capacity(0);
        cache[0] = (seq_t)1; /* a(0) = 1 */
        max_computed = 0;
    }

    /* return cached value if already known */
    if (n <= max_computed) return cache[n];

    /* compute all missing terms up to n */
    for (idx_t m = max_computed + 1; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 0; k < m; ++k) {
            /* binomial(m + k, m - k - 1) * 2^(m-k-1) * a(k) */
            seq_t bin   = binomial(m + k, m - k - 1);
            seq_t pw    = pow2(m - k - 1);
            seq_t term  = bin * pw;
            term        = term * cache[k];
            sum         = sum + term;
        }
        ensure_capacity(m);
        cache[m] = sum;
    }
    max_computed = n;
    return cache[n];
}
