/*
 * A368322  Expansion of e.g.f. exp(2*x) / (4 - 3*exp(x)).
 * https://oeis.org/A368322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n + 3 * Sum_{k=1..n} binomial(n,k) * a(n-k), a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;          /* number of computed entries */
    /* Ensure cache includes 0 */
    if (cap == 0) {
        cap = 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed */
        cache[0] = 1;          /* a(0) = 1 */
    }
    /* Grow cache if necessary */
    if (n >= cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;    /* allocation failed */
        cache = tmp;
        /* Compute new entries */
        for (size_t i = cap; i <= (size_t)n; ++i) {
            idx_t idx = (idx_t)i;
            if (i == 0) {        /* already set */
                continue;
            }
            seq_t pow2_i   = pow2(idx);   /* 2^i */
            seq_t sum      = 0;
            seq_t binom    = 1;           /* binomial(i,0) */
            /* Sum over j = 0 .. i-1 of binom(i,j) * a(j)   */
            for (idx_t j = 0; j < idx; ++j) {
                sum += binom * cache[j];
                /* Update binom for next j */
                if (j + 1 < idx) {
                    seq_t numer = (seq_t)(idx - j);
                    seq_t denom = (seq_t)(j + 1);
                    binom = (binom * numer) / denom;
                }
            }
            cache[i] = pow2_i + (seq_t)3 * sum;
        }
        cap = new_cap;
    }
    return cache[n];
}
