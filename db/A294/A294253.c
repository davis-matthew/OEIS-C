/*
 * A294253  E.g.f.: exp((1+x)*(1+x^2)*(1+x^3)*(1+x^4)*(1+x^5) - 1).
 * https://oeis.org/A294253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exp((1+x)(1+x^2)(1+x^3)(1+x^4)(1+x^5)-1) Recurrence: a(n)
 *            = Σ_{k=1}^{15} k·d_k·(n−1)_{k−1}·a(n−k) with d =
 *            [0,1,1,2,2,3,3,3,3,3,3,2,2,1,1,1] (indices 0..15) and a(0)=1,
 *            a(1)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const unsigned int d[16] = {
    0, 1, 1, 2, 2, 3, 3, 3, 3, 3, 3, 2, 2, 1, 1, 1
};

static seq_t *cache = NULL;
static idx_t cached = 0;   /* largest index for which cache[i] is valid */

seq_t a(idx_t n)
{
    /* Allocate initial cache if necessary */
    if (cache == NULL) {
        size_t sz = (size_t)(n + 1);
        if (sz < 1) sz = 1;
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cached = 0;
    }

    /* Grow cache to accommodate n */
    if (n > cached) {
        size_t new_sz = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    }

    /* Compute missing values up to n */
    for (idx_t i = cached + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (int k = 1; k <= 15 && k <= i; ++k) {
            /* falling factorial (i-1)_{k-1} */
            seq_t fac = 1;
            for (idx_t t = 1; t < (idx_t)k; ++t) {
                fac *= (seq_t)(i - t);
            }
            unsigned int ck = k * d[k];
            seq_t term = (seq_t)ck * fac * cache[i - k];
            sum += term;
        }
        cache[i] = sum;
    }

    cached = n;
    return cache[n];
}
