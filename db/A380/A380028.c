/*
 * A380028  Expansion of e.g.f. sqrt(1 - 2*x*exp(x)).
 * https://oeis.org/A380028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(n)=(-2*n - Σ_{i=1}^{n-1} binom(n,i)*a(i)*a(n-i))/2
 *            Derived from (sqrt(1‑2*x*exp(x)))^2 = 1‑2*x*exp(x)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 9, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n) {
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t computed = 0;   /* number of values computed: cache[0..computed-1] */

    if (n == 0)
        return (seq_t)1;

    /* enlarge cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (n + 1) * 2;
        if (new_cap <= (size_t)n + 1)
            new_cap = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;   /* out of memory */
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    if (computed == 0) {
        cache[0] = (seq_t)1;
        computed = 1;
    }

    for (idx_t k = computed; k <= n; ++k) {
        seq_t sum = (seq_t)0;
        idx_t half = k / 2;
        for (idx_t i = 1; i <= half; ++i) {
            seq_t bin = binom((idx_t)k, (idx_t)i);
            seq_t term = cache[i] * cache[(idx_t)(k - i)];
            term *= bin;
            if (i < (idx_t)(k - i)) {
                sum += term;
                sum += term;
            } else {
                sum += term;
            }
        }
        seq_t two_n = (seq_t)((seq_t)2 * (seq_t)k);
        seq_t numerator = -two_n;
        numerator -= sum;
        cache[k] = numerator / (seq_t)2;
    }

    computed = n + 1;
    return cache[n];
}
