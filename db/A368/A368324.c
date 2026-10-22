/*
 * A368324  Expansion of e.g.f. exp(4*x) / (4 - 3*exp(x)).
 * https://oeis.org/A368324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^n + 3 * Sum_{k=1..n} binomial(n,k) * a(n-k) Initial
 *            value: a(0) = 1 Implementation uses memoisation and an O(n^2)
 *            convolution with incremental binomial calculation
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t next_compute = 0;
    static seq_t pow4_last;  /* 4^(next_compute-1), after initialisation 1 */

    /* initialise the cache on first call */
    if (cache == NULL) {
        grow_seq(&cache, &cache_cap, 1);
        cache[0] = 1;            /* a(0) */
        pow4_last = (seq_t)1;    /* 4^0 */
        next_compute = 1;        /* next term to compute */
    }

    /* compute missing terms up to n */
    while (next_compute <= n) {
        idx_t i = next_compute;          /* current index */
        seq_t sum   = 0;                 /* Σ_{k=0}^{i-1} C(i,k) * a(k) */
        seq_t binom = 1;                 /* C(i,0) */

        for (idx_t k = 0; k < i; ++k) {
            if (k > 0) {
                /* C(i,k) = C(i,k-1) * (i-k+1) / k  (exact division) */
                binom = binom * (seq_t)(i - k + 1) / (seq_t)k;
            }
            sum += binom * cache[k];
        }

        /* 4^i = 4 * 4^(i-1) */
        seq_t pow4 = pow4_last * (seq_t)4;
        cache[i] = pow4 + (seq_t)3 * sum;

        pow4_last = pow4;          /* update for next iteration */
        ++next_compute;
    }

    return cache[n];
}
