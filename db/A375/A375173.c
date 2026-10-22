/*
 * A375173  Expansion of e.g.f. exp( (1/(1 - 4*x)^(1/2) - 1)/2 ).
 * https://oeis.org/A375173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*(2*n - 3)*a(n-1) - (48*n^2 - 192*n + 191)*a(n-2) +
 *            32*(n-3)*(n-2)*(2*n - 5)*a(n-3) with initial values a(0)=1,
 *            a(1)=1, a(2)=7, a(3)=79
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* memoized table: */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    /* return already computed value if present */
    if (n < cache_cap && cache != NULL)
        return cache[n];

    /* allocate / grow cache to hold up to index n */
    size_t new_cap = (size_t)n + 1;
    seq_t *old_cache = cache;
    cache = (seq_t *)realloc(old_cache, new_cap * sizeof(seq_t));
    if (!cache && new_cap > 0)   /* allocation failed */
        return (seq_t)0;

    /* compute values from current cache_cap up to n */
    for (idx_t i = cache_cap; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)1;
        } else if (i == 2) {
            cache[i] = (seq_t)7;
        } else if (i == 3) {
            cache[i] = (seq_t)79;
        } else {
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];

            /* coefficients are small enough to fit in 64‑bit unsigned */
            unsigned long long c1 = 6ULL * ((2ULL * i) - 3ULL);
            unsigned long long c2 = 48ULL * i * i - 192ULL * i + 191ULL;
            unsigned long long c3 = 32ULL * (i - 3ULL) * (i - 2ULL) *
                                    ((2ULL * i) - 5ULL);

            seq_t term1 = a1 * (seq_t)c1;
            seq_t term2 = a2 * (seq_t)c2;
            seq_t term3 = a3 * (seq_t)c3;

            cache[i] = term1 - term2 + term3;
        }
    }

    cache_cap = new_cap;
    return cache[n];
}
