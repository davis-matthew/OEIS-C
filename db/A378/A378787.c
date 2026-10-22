/*
 * A378787  G.f. A(x) satisfies A(x) = ( 1 + x * (1 + x*A(x)^2)^3 )^2.
 * https://oeis.org/A378787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(4*(n-k)+2,k) *
 *            binomial(3*k,n-k)/(2*(n-k)+1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function memoizes computed values so each a(n) is evaluated only once. */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;       /* array of seq_t values */
    static size_t cache_capacity = 0; /* allocated capacity */
    static size_t computed_size = 0;  /* number of terms already computed */

    /* Ensure cache can hold up to n */
    if (n + 1 > cache_capacity) {
        size_t new_cap = cache_capacity ? cache_capacity * 2 : 64;
        while (new_cap <= n) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_capacity = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = computed_size; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k <= i; ++k) {
            seq_t b1 = binomial(4 * (i - k) + 2, k);
            seq_t b2 = binomial(3 * k, i - k);
            seq_t prod = b1 * b2;
            seq_t denom = 2 * (i - k) + 1;
            sum += prod / denom;
        }
        cache[i] = sum;
    }
    computed_size = n + 1;

    return cache[n];
}
