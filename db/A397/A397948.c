/*
 * A397948  G.f. A(x) satisfies A(x) = 1 + x * A(x^4/(1 - x)^4) / (1 - x)^3.
 * https://oeis.org/A397948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊(n-1)/4⌋} binomial(n+1, 4k+2) * a(k) a(0) = 1
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 30, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>         /* malloc, realloc */
#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) returns seq_t */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = (idx_t)-1;

    /* ensure cache is large enough */
    if (cache == NULL) {
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        cache_cap = n + 1;
        if (cache == NULL) return (seq_t)0;   /* out of memory; will never happen */
        /* initialise to 0 */
        for (size_t i = 0; i < cache_cap; ++i) cache[i] = (seq_t)0;
    } else if (cache_cap <= n) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* out of memory */
        /* initialise new elements to 0 */
        for (size_t i = cache_cap; i < (size_t)(n + 1); ++i) cache[i] = (seq_t)0;
        cache_cap = n + 1;
    }

    /* first value */
    if (last_computed == (idx_t)-1) {
        cache[0] = (seq_t)1;
        last_computed = 0;
        if (n == 0) return cache[0];
    }

    /* compute any missing values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        idx_t max_k = (i == 0) ? (idx_t)-1 : (i - 1) / 4;  /* for i=0, loop none */
        for (idx_t k = 0; k <= max_k; ++k) {
            seq_t comb = binomial(i + 1, 4 * k + 2);
            sum += comb * cache[k];
        }
        cache[i] = sum;
    }
    last_computed = n;

    return cache[n];
}
