/*
 * A385762  G.f. A(x) satisfies A(x) = 1/(1 - x*A(x) - x^3*A''(x)).
 * https://oeis.org/A385762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=0..n-1} (1 - k + k^2) * a(k) * a(n-1-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_size = 0;

    /* Initial allocation if needed */
    if (cache == NULL) {
        cache_cap = (size_t)(n + 1);
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed */
        cache[0]   = (seq_t)1;
        cache_size = 1;
    }
    /* Grow cache when a larger n is requested */
    else if ((size_t)(n + 1) > cache_cap) {
        cache_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
    }

    /* Compute missing terms */
    if (n >= cache_size) {
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k < i; ++k) {
                seq_t coeff = (seq_t)1 - (seq_t)k + (seq_t)k * (seq_t)k;
                seq_t prod  = cache[k] * cache[i - 1 - k];
                sum += coeff * prod;
            }
            cache[i] = sum;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
