/*
 * A352646  Expansion of e.g.f. 1/(1 - 2 * x * cos(x)).
 * https://oeis.org/A352646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Σ_{k=0..⌊(n−1)/2⌋} (−1)^k * (2k+1) * C(n,2k+1) *
 *            a(n−2k−1) with a(0) = 1. The function uses a cache so calls may
 *            be in any order
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* number of slots allocated */
    static size_t computed   = 0;  /* values 0..computed‑1 are valid */

    /* Return cached value if already known */
    if ((size_t)n < computed)
        return cache[(size_t)n];

    /* Grow the cache if necessary */
    if (cache_cap <= (size_t)n) {
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * ((size_t)n + 1));
        if (!tmp) { /* allocation failure */
            exit(1);
        }
        cache     = tmp;
        cache_cap = (size_t)n + 1;
    }

    /* Compute any missing terms up to n */
    for (size_t idx = computed; idx <= (size_t)n; ++idx) {
        idx_t i = (idx_t)idx;

        if (i == 0) {              /* base case */
            cache[idx] = 1;
            continue;
        }

        seq_t pos = 0, neg = 0;
        idx_t max_k = (i - 1) / 2;
        for (idx_t k = 0; k <= max_k; ++k) {
            idx_t m = 2 * k + 1;                  /* 2k+1 */
            seq_t bin = binomial(i, m);           /* C(i, 2k+1) */
            seq_t term = bin;
            term *= cache[idx - (size_t)m];       /* a(i‑m) */
            term *= m;                            /* multiply by (2k+1) */

            if ((k & 1) == 0)
                pos += term;
            else
                neg += term;
        }

        cache[idx] = (pos - neg) * 2;             /* final 2*sum */
    }

    computed = (size_t)n + 1;
    return cache[(size_t)n];
}
