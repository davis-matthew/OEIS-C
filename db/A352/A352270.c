/*
 * A352270  Expansion of e.g.f. 1/(2 - exp(x) + log(1 - x)).
 * https://oeis.org/A352270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=∑_{k=1..n} ((k-1)!+1)*binomial(n,k)*a(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* cache for previously computed terms */
static seq_t *cache   = NULL;   /* a[0..] */
static size_t cache_cap     = 0;   /* allocated capacity for cache   */
static idx_t  last_computed = 0;   /* highest n that has been computed */

/* enlarge cache to hold indices up to new_cap-1 */
static void ensure_cache(size_t new_cap)
{
    if (cache_cap >= new_cap) return;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return; /* allocation failure – unlikely */
    cache = tmp;
    cache_cap = new_cap;
}

/* main sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    /* 1. initialise caches on first call */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;
        last_computed = 0;
    }

    /* 2. grow caches if needed */
    ensure_cache((size_t)(n + 1));

    /* 3. compute missing values */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;          /* C(i,0) */
        seq_t fact_kminus1 = (seq_t)1;   /* (k-1)! for k=1 is 0! = 1 */

        for (idx_t k = 1; k <= i; ++k) {
            /* update binomial to C(i,k) */
            binom = binom * (seq_t)(i - k + 1) / (seq_t)k;

            /* term = ((k-1)! + 1) * C(i,k) * a(i-k) */
            seq_t term = (fact_kminus1 + (seq_t)1) * binom;
            term = term * cache[i - k];
            sum = sum + term;

            /* prepare (k)! for next iteration */
            fact_kminus1 *= (seq_t)k;
        }

        cache[i] = sum;
    }
    last_computed = n;

    return cache[n];
}
