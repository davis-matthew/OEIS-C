/*
 * A335577  a(0) = 1; a(n) = -Sum_{k=1..n} binomial(n,k) * k^2 * a(n-k).
 * https://oeis.org/A335577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -∑_{k=1..n} binomial(n,k) * k² * a(n−k) (OEIS A335577)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 19, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache   = NULL;
    static idx_t  cached = 0;      /* number of terms already computed (last index + 1) */
    static size_t cap     = 0;     /* allocated capacity (in elements) */

    /* allocate initial memory if necessary */
    if (cap == 0) {
        cap = (size_t)8;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure – return 0 as a fallback */
    }

    /* grow cache if required */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure – return 0 as a fallback */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = cached; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;              /* base case a(0) = 1 */
            continue;
        }
        seq_t sum   = 0;
        seq_t binom = 1;              /* binomial(i,0) */
        for (idx_t k = 1; k <= i; ++k) {
            /* update binomial coefficient: binom = binom * (i-k+1) / k */
            binom = binom * (seq_t)(i - k + 1);
            binom = binom / (seq_t)k;
            seq_t ksq  = (seq_t)k * (seq_t)k;
            seq_t term = binom * ksq;
            term = term * cache[i - k];
            sum  = sum + term;
        }
        cache[i] = -sum;
    }
    cached = n + 1;   /* we now have terms 0..n stored */

    return cache[n];
}
