/*
 * A289067  Recurrence a(n+2) = Sum_{k=0..n} binomial(n,k)*a(k)*a(n+1-k) with
 *          a(0)=3, a(1)=-1.
 * https://oeis.org/A289067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n+2) = Sum_{k=0..n} binomial(n,k)*a(k)*a(n+1-k), a(0)=3,
 *            a(1)=-1
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 13, i64 n <= 22, i128 n <= 36,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"

seq_t a(idx_t n) {
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t last_computed = 1; /* 0 and 1 are stored initially */

    /* ensure sufficient space and set initial values on first call */
    if (cache == NULL) {
        ensure_cache(n + 1, &cache, &cap);
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)-1;
        last_computed = 1;
    } else {
        ensure_cache(n + 1, &cache, &cap);
    }

    /* iterative computation of missing terms */
    while (last_computed < n) {
        idx_t idx = last_computed + 1;          /* new index to compute */
        idx_t m   = idx - 2;                    /* n in the recurrence */

        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;                 /* binomial(m,0) */

        for (idx_t k = 0; k <= m; ++k) {
            seq_t prod = cache[k] * cache[m + 1 - k];
            prod *= binom;
            sum += prod;

            /* update binomial for next k */
            if (k < m) {
                seq_t num = (seq_t)(m - k);
                seq_t den = (seq_t)(k + 1);
                binom = binom * num / den;
            }
        }

        cache[idx] = sum;
        last_computed = idx;
    }

    return cache[n];
}
