/*
 * A355236  E.g.f. A(x) satisfies A'(x) = 1 + (exp(x) - 1) * A(2*x)/2.
 * https://oeis.org/A355236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1; a(n+1)= Σ_{k=1}^{n-1} 2^{k-1}*binomial(n,k)*a(k)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t max_cached = 0;
static seq_t *cache      = NULL;
static size_t cache_len  = 0;

seq_t a(idx_t n)
{
    /* Ensure cache is at least size 2 and contains initial values */
    if (cache == NULL) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) return 0;          /* OOM, unlikely */
        cache[0] = 0;
        cache[1] = 1;
        max_cached = 1;
    }

    /* If already computed, return cached value */
    if (n <= max_cached)
        return cache[n];

    /* Extend cache if necessary */
    if (n + 1 > cache_len) {
        cache_len = n + 1;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
    }

    /* Compute new values */
    for (idx_t m = max_cached + 1; m <= n; ++m) {
        if (m <= 2) {                 /* a(2) = 0, a(0/1) already set */
            cache[m] = 0;
            continue;
        }

        idx_t nv   = m - 1;           /* n in recurrence: a(m) = Σ_{k=1}^{m-2} 2^{k-1}*C(nv,k)*a(k) */
        seq_t sum  = 0;
        seq_t pow2 = 1;               /* 2^{k-1} for current k */
        seq_t binom = nv;             /* binomial(nv,1) */

        for (idx_t k = 1; k <= m - 2; ++k) {
            seq_t term = binom;
            term *= cache[k];
            term *= pow2;
            sum += term;

            /* Advance to next k */
            if (k < m - 2) {
                seq_t num = (seq_t)(nv - k);            /* nv - k */
                binom = binom * num / (k + 1);          /* binomial(nv,k+1) */
                pow2 <<= 1;                              /* multiply by 2 */
            }
        }
        cache[m] = sum;
    }

    max_cached = n;
    return cache[n];
}
