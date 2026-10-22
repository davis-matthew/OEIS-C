/*
 * A352147  Expansion of e.g.f. 1/(exp(x) + log(1 + x)).
 * https://oeis.org/A352147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; a(n)=∑_{k=1..n} ((-1)^k*(k-1)!-1) * C(n,k) * a(n-k)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 16, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms (including a(0)) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of computed terms */

    /* allocate or extend cache if needed */
    if (cache == NULL) {
        cache = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache_len = 0;
    } else if ((idx_t)cache_len < n + 1) {
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return cache[cache_len];
        cache = tmp;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t res   = (seq_t)0;
        seq_t binom = (seq_t)1;   /* C(i,0) starts as 1 */
        seq_t fact  = (seq_t)1;   /* (k-1)! for k=1 */

        for (idx_t k = 1; k <= i; ++k) {
            /* update binomial coefficient to C(i,k) */
            binom = binom * (seq_t)(i - k + 1) / (seq_t)k;

            /* compute g_k = ((-1)^k*(k-1)! - 1) */
            seq_t gk = ((k & 1) == 0) ? (fact - (seq_t)1)
                                      : (-fact - (seq_t)1);

            /* term = gk * binom * a(i-k) */
            seq_t term = binom * gk;
            term = term * cache[i - k];
            res += term;

            /* update factorial for next iteration */
            fact *= (seq_t)k;      /* fact now equals k! for next k */
        }

        cache[i] = res;
    }
    cache_len = n + 1;

    return cache[n];
}
