/*
 * A386505  a(0) = 1; a(n) = a(n-1) + Sum_{k=0..n-1} (1 + k) * k^2 *
 *          binomial(n-1,k) * a(k) * a(n-1-k).
 * https://oeis.org/A386505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = a(n-1) + Σ_{k=0}^{n-1} (1+k) * k^2 *
 *            binomial(n-1,k) * a(k) * a(n-1-k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_computed = 0;   /* highest n initialized in cache */

    /* first time: create cache[0] = 1 */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed */
        cache[0] = (seq_t)1;
        cache_cap = 1;
        max_computed = 0;
    }

    /* if the requested n is already known, return it */
    if (n <= max_computed)
        return cache[n];

    /* grow the cache to hold indices up to n */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        if (new_cap < (size_t)(n + 1))
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute values for indices max_computed+1 .. n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = 0;

        /* iteratively compute binomial coefficient C(i-1, k) */
        seq_t comb = (seq_t)1;  /* C(i-1,0) */
        for (idx_t k = 0; k < i; ++k) {
            seq_t kk = (seq_t)k * (seq_t)k;                 /* k^2 */
            seq_t factor = kk * ((seq_t)k + (seq_t)1);     /* (1+k)*k^2 */

            /* term = factor * comb * a(k) * a(i-1-k) */
            seq_t term = factor * comb * cache[k] * cache[i - 1 - k];
            sum += term;

            /* update comb to C(i-1, k+1) if needed */
            if (k + 1 < i) {
                comb = (comb * ((seq_t)(i - 1 - k))) / ((seq_t)(k + 1));
            }
        }

        cache[i] = cache[i - 1] + sum;
    }

    max_computed = n;
    return cache[n];
}
