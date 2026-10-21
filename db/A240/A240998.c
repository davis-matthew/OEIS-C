/*
 * A240998  G.f. satisfies: A(x)^2 = x + A(x + 2*x^2).
 * https://oeis.org/A240998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (derived from A(x)^2 = x + A(x+2*x^2))
   a(0) = 1
   For n ≥ 1:
      a(n) = Σ_{k=ceil(n/2)}^{n-1} a(k) * binom(k, n-k) * 2^{n-k}
             - Σ_{i=1}^{n-1} a(i) * a(n-i)
   with a(1) = 1 coming from the term x on the right-hand side.
   The function keeps a static cache to answer subsequent calls in
   amortised O(n) time.                                         */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

static seq_t *cache = NULL;          /* cache of computed a(k) */
static size_t cache_cap = 0;         /* capacity of the cache */
static idx_t  cache_max = (idx_t)-1; /* largest index computed */

/* ensure cache has room for indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
        /* initialise first element if cache was empty */
        if (cache_max == (idx_t)-1) {
            cache[0] = (seq_t)1;
            cache_max = 0;
        }
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n <= cache_max) return cache[n];

    for (idx_t k = cache_max + 1; k <= n; ++k) {
        /* Σ_{i=1}^{k-1} a(i) * a(k-i) */
        seq_t sum_cross = 0;
        for (idx_t i = 1; i <= k - 1; ++i) {
            sum_cross += cache[i] * cache[k - i];
        }

        /* Σ_{j=ceil(k/2)}^{k-1} a(j) * binom(j, k-j) * 2^{k-j} */
        seq_t sum_shift = 0;
        idx_t start = (k + 1) / 2;          /* ceil(k/2) */
        for (idx_t j = start; j <= k - 1; ++j) {
            idx_t r = k - j;                /* n - j = k - j */
            seq_t bin = binomial(j, r);
            seq_t two = pow2(k - j);
            seq_t temp = cache[j] * bin;
            temp *= two;
            sum_shift += temp;
        }

        seq_t val = sum_shift - sum_cross;
        if (k == 1) val += (seq_t)1;        /* a(1) = 1 */
        cache[k] = val;
    }

    cache_max = n;
    return cache[n];
}
