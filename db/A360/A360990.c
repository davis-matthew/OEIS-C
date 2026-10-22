/*
 * A360990  E.g.f. satisfies A(x) = exp(x / A(-x)^3).
 * https://oeis.org/A360990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-3*n + 3*k + 1)^(k-1) * (3*k)^(n-k) *
 *            binomial(n,k)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 13, i128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;        /* cache of computed terms */
    static idx_t max_cached = (idx_t)-1; /* no terms cached initially */

    if (n == 0) return 1;              /* a(0) = 1 */

    if (cache == NULL) {               /* initialise cache with a(0) */
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = 1;
        max_cached = 0;
    }

    if (n <= max_cached)               /* value already computed */
        return cache[n];

    /* extend cache to contain terms up to n */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) {                   /* allocation failed */
        exit(EXIT_FAILURE);
    }
    cache = new_cache;

    /* compute missing terms incrementally */
    for (idx_t m = max_cached + 1; m <= n; ++m) {
        seq_t sum = 0;

        /* k = 0 gives 0 for m > 0; skip it */
        for (idx_t k = 1; k <= m; ++k) {
            /* base1 = -3*m + 3*k + 1 */
            seq_t base1 = (seq_t)(-3 * (seq_t)m + 3 * (seq_t)k + 1);
            seq_t term1 = big_pow(base1, (seq_t)(k - 1));

            /* base2 = 3*k */
            seq_t base2 = (seq_t)(3 * (seq_t)k);
            seq_t term2 = big_pow(base2, (seq_t)(m - k));

            seq_t bin = binomial(m, k);

            seq_t term = term1 * term2 * bin;
            sum += term;
        }
        cache[m] = sum;
    }

    max_cached = n;
    return cache[n];
}
