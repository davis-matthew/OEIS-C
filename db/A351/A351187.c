/*
 * A351187  G.f. A(x) satisfies: A(x) = 1 + x + x^2 * A(x/(1 + 6*x)) / (1 +
 *          6*x).
 * https://oeis.org/A351187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ∑_{k=0}^{n-2} binom(n-2,k) * (-6)^k * a(n-k-2) Initial
 *            values: a(0)=1, a(1)=1
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Simple memoised DP using the defining recurrence.  */
    /* Cache allocated on first call and expanded as needed. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of initialised entries */

    /* initialise first two terms */
    if (!cache) {
        cache_size = 2;
        cache = (seq_t*)calloc((size_t)cache_size, sizeof(seq_t));
        if (!cache) return 0;         /* allocation failed */
        cache[0] = 1;
        cache[1] = 1;
    }

    /* extend cache up to 'n' if required */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t*)realloc((void*)cache, (size_t)new_size * sizeof(seq_t));
        if (!new_cache) return 0;     /* allocation failed */
        cache = new_cache;
        /* compute missing values */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 2) {              /* a(0), a(1) already set */
                cache[i] = 1;
                continue;
            }
            idx_t m = i - 2;          /* loop length for this n  */
            seq_t sum = 0;
            seq_t binom = 1;          /* binom(m,0) = 1 */
            seq_t pow   = 1;          /* (-6)^0  = 1 */

            for (idx_t k = 0; k <= m; ++k) {
                /* term = binom(m,k) * (-6)^k * a[i-k-2] */
                seq_t term = binom;
                term *= pow;
                term *= cache[i - k - 2];
                sum += term;

                if (k < m) {
                    /* update binom for next k : binom = binom * (m-k) / (k+1) */
                    seq_t num = binom * (seq_t)(m - k);
                    binom = num / (seq_t)(k + 1);

                    /* update power: pow = pow * (-6) */
                    pow *= (seq_t)(-6);
                }
            }
            cache[i] = sum;
        }
        cache_size = new_size;
    }

    return cache[n];
}
