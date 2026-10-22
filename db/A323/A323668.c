/*
 * A323668  Expansion of e.g.f. exp(exp(2*x)*(BesselI(0,2*x) + BesselI(1,2*x))
 *          - 1).
 * https://oeis.org/A323668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = Σ_{k=1..n} A001700(k) * binomial(n-1, k-1) *
 *            a(n-k) where A001700(k) = binomial(2*k+1, k) (OEIS A323668)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* provides: seq_t binomial(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    /* static cache for computed a values */
    static seq_t *cache      = NULL;   /* cache[0..] */
    static size_t cache_cap  = 0;      /* allocated capacity */
    static idx_t  cache_len  = 0;      /* number of terms already computed */

    /* static cache for A001700(k)=binomial(2*k+1,k) values */
    static seq_t *B          = NULL;   /* B[k] = binomial(2*k+1, k) */
    static size_t B_cap      = 0;      /* largest k already stored */

    /* Ensure cache can hold a[n] */
    if (n + 1 > cache_cap) {
        size_t new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialise a[0] */
    if (cache_len == 0) {
        cache[0] = 1;
        cache_len = 1;
    }

    /* Compute missing terms up to n */
    for (idx_t m = cache_len; m <= n; ++m) {
        /* Compute A001700 values up to k=m (if needed) */
        if ((size_t)m > B_cap) {
            size_t new_B_cap = m + 1;
            B = (seq_t *)realloc(B, new_B_cap * sizeof(seq_t));
            for (idx_t k = B_cap + 1; k <= m; ++k) {
                B[k] = binomial(2 * k + 1, k);
            }
            B_cap = m;
        }

        /* Evaluate the recurrence for a[m] */
        seq_t sum = 0;                /* a[m] */
        seq_t binom_iter = 1;          /* binomial(m-1, 0) */

        for (idx_t k = 1; k <= m; ++k) {
            /* term = B[k] * binom_iter * cache[m-k] */
            seq_t term = B[k] * binom_iter;
            term = term * cache[m - k];
            sum = sum + term;

            /* update binom_iter: binomial(m-1, k) = binomial(m-1, k-1)*(m-k)/k */
            if (k < m) {
                seq_t tmp = binom_iter * (seq_t)(m - k);
                binom_iter = tmp / (seq_t)k;
            }
        }

        cache[m] = sum;
    }

    cache_len = n + 1;
    return cache[n];
}
