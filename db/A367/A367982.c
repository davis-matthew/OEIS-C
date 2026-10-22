/*
 * A367982  Expansion of e.g.f. exp(-2*x) / (2 - exp(4*x)).
 * https://oeis.org/A367982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-2)^n + Σ_{k=1..n} binomial(n,k) * 4^k * a(n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"            /* provides seq_mul(seq_t, seq_t) */

seq_t a(idx_t n)
{
    /* Cache of already computed values.
       cache[0] = a(0), cache[1] = a(1), … */
    static seq_t *cache      = NULL;   /* dynamic array of seq_t */
    static size_t cache_cap  = 0;       /* allocated capacity (elements) */
    static idx_t  cache_last = 0;       /* largest n for which cache[n] is valid */
    static seq_t pow2        = 1;       /* 2^m at the start of computing a(m) */

    /* First call: allocate minimal capacity for element 0 */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return 0;          /* malloc failed – return 0 */
        cache[0]  = 1;                 /* a(0) = 1 */
        cache_last = 0;
    }

    /* Grow cache if necessary for indices up to n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return 0;           /* realloc failed – return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t m = cache_last + 1; m <= n; ++m) {
        /* Prepare 2^m for the (-2)^m term.
           At the beginning of the loop pow2 holds 2^(m-1); */
        pow2 = seq_mul(pow2, (seq_t)2);

        seq_t sum   = 0;   /* Σ_{k=1..m} binomial(m,k) * 4^k * a(m-k) */
        seq_t binom = 1;   /* binomial(m,0) */
        seq_t pow4  = 1;   /* 4^0 */

        for (idx_t k = 1; k <= m; ++k) {
            /* binom = C(m,k) */
            binom = (binom * (seq_t)(m - k + 1)) / (seq_t)k;

            /* pow4 = 4^k */
            pow4 = seq_mul(pow4, (seq_t)4);

            /* term = binom * pow4 * a(m-k) */
            seq_t term = seq_mul(binom, pow4);
            term       = seq_mul(term, cache[m - k]);

            sum += term;
        }

        /* Add (-2)^m = ((m&1) ? -pow2 : +pow2) */
        if (m & 1)
            sum -= pow2;
        else
            sum += pow2;

        cache[m] = sum;
    }

    cache_last = n;
    return cache[n];
}
