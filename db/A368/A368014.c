/*
 * A368014  Expansion of e.g.f. 1/(1 - exp(x) + exp(3*x)).
 * https://oeis.org/A368014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1 a(n) = Σ_{k=1..n} (1 - 3^k) * binomial(n,k) * a(n-k)
 *            for n ≥ 1
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 11, i64 n <= 18, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* allocate or extend cache as needed */
    if (cache_size <= n) {
        size_t newsize = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, newsize * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure, return 0 */
        cache = tmp;
        /* initialise new slots */
        for (idx_t i = cache_size; i < newsize; ++i) cache[i] = 0;
        /* compute missing terms sequentially */
        for (idx_t i = cache_size; i < newsize; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
                continue;
            }
            seq_t sum = 0;
            seq_t binom = 1;          /* binomial(i,0) */
            seq_t pow3  = 1;          /* 3^0 */
            for (idx_t k = 1; k <= i; ++k) {
                /* update 3^k */
                pow3 *= (seq_t)3;
                /* binomial(i, k) = binomial(i, k-1) * (i-k+1) / k */
                binom = (binom * (seq_t)(i - k + 1)) / (seq_t)k;
                /* term = (1 - 3^k) * binomial * a(i-k) */
                seq_t term = ((seq_t)1 - pow3) * binom * cache[i - k];
                sum += term;
            }
            cache[i] = sum;
        }
        cache_size = newsize;
    }

    return cache[n];
}
