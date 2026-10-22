/*
 * A385923  E.g.f. A(x) satisfies A(x) = exp(x*A(x) + x^6*A'''''(x)).
 * https://oeis.org/A385923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} (1 + k) * (1 + 24*k - 50*k^2 + 35*k^3 -
 *            10*k^4 + k^5) * binomial(n-1,k) * a(k) * a(n-1-k) * a(0) = 1,
 *            a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* ---------- Memoisation cache ---------- */
static seq_t *cache = NULL;      /* pointer to cached values    */
static size_t cache_cap = 0;     /* current capacity in entries */

/* Ensure the cache can hold indices up to `n`.  New entries are zeroed. */
static void
ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    if (new_cap < needed)
        new_cap = needed;

    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) /* realloc failed – abort */
        abort();

    /* initialise new slots to 0 */
    for (size_t i = cache_cap; i < new_cap; ++i)
        tmp[i] = 0;

    cache = tmp;
    cache_cap = new_cap;
}

/* ---------- Sequence generator ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;           /* a(0) = 1 */

    /* make sure cache can hold all indices up to n */
    ensure_cache(n);

    /* base values */
    if (cache[0] == 0)
        cache[0] = (seq_t)1;
    if (cache[1] == 0)
        cache[1] = (seq_t)1;

    /* compute missing terms up to n */
    for (idx_t m = 2; m <= n; ++m) {
        if (cache[(size_t)m] != (seq_t)0)    /* already computed */
            continue;

        seq_t sum = 0;

        for (idx_t k = 0; k < m; ++k) {
            seq_t kk = (seq_t)k;

            /* polynomial p(k) = 1 + 24k - 50k^2 + 35k^3 - 10k^4 + k^5 */
            seq_t p = 1;
            p += 24 * kk;
            p -= 50 * kk * kk;
            p += 35 * kk * kk * kk;
            p -= 10 * kk * kk * kk * kk;
            p += kk * kk * kk * kk * kk;

            /* factor (1 + k) * p(k) */
            seq_t factor = (kk + 1) * p;

            /* binomial coefficient */
            seq_t bin = binomial(m - 1, k);

            /* combine: (1 + k)*p(k)*C(m-1,k)*a(k)*a(m-1-k) */
            seq_t term = factor;
            term *= bin;
            term *= cache[(size_t)k];
            term *= cache[(size_t)(m - 1 - k)];

            sum += term;
        }
        cache[(size_t)m] = sum;
    }

    return cache[(size_t)n];
}
