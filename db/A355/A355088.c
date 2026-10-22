/*
 * A355088  E.g.f. A(x) satisfies A(x) = 1 + (exp(x) - 1) * A(3*x).
 * https://oeis.org/A355088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} 3^k * binomial(n,k) * a(k), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n) {
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = 0; /* largest index with computed value */

    /* Allocate or grow cache to hold indices up to n */
    ensure_cache(n + 1, &cache, &cache_cap);

    /* Initialize a(0) if needed */
    if (last_computed == 0 && cache[0] == 0) {
        cache[0] = 1;
        last_computed = 0;
    }

    /* Compute any missing values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        seq_t coeff = 1;          /* coefficient = binom(i,0) * 3^0 = 1 */

        for (idx_t k = 0; k < i; ++k) {
            /* add term for current k */
            sum += coeff * cache[k];

            /* prepare coefficient for next k (if any) */
            if (k + 1 < i) {
                coeff = coeff * (seq_t)(i - k);      /* multiply by (i - k) */
                coeff = coeff / (seq_t)(k + 1);      /* exact division by (k + 1) */
                coeff = coeff * (seq_t)3;            /* multiply by 3^1 */
            }
        }

        cache[i] = sum;
        last_computed = i;
    }

    return cache[n];
}
