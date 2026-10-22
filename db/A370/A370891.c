/*
 * A370891  G.f. satisfies A(x) = ( 1 + x * A(x)^(1/4) * (1 + A(x)^(3/4)) )^2.
 * https://oeis.org/A370891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n > 0, * a(n) = 4 * Σ_{k=0..n, n+3k even}
 *            binomial(n,k) * * binomial((n+3k)/2 + 2, n) / ((n+3k)/2 + 2) *
 *            The sum is over those k for which (n+3k) is even, so that the *
 *            middle binomial is defined with an integer upper index. * Each
 *            term is an integer because 4 * binomial(n,k) * bino...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"

/* Static cache for computed a(n) values. */
static seq_t *cache = NULL;          /* array of precomputed values */
static idx_t cache_cap = 0;          /* current allocation size */
static idx_t cache_max = 0;          /* largest n computed so far */

/* Allocate or resize the cache array. */
static void ensure_cache(idx_t n)
{
    if (n >= cache_cap) {
        idx_t new_cap = n + 1;
        if (new_cap < 2) new_cap = 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        for (idx_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* offset 0 : first term is 1 */
    if (n == 0) return (seq_t)1;

    /* Extend cache and compute new terms up to n. */
    ensure_cache(n);
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k <= i; ++k) {
            /* Only include terms where (i + 3k) is even. */
            idx_t sum_ik = i + 3 * k;
            if (sum_ik & 1) continue;          /* skip odd */
            idx_t m = (sum_ik >> 1) + 2;        /* m = (i+3k)/2 + 2 */

            seq_t bin1 = binomial(i, k);        /* C(i,k) */
            seq_t bin2 = binomial(m, i);        /* C(m,i) */

            seq_t term = bin1;
            term *= bin2;
            term *= (seq_t)4;
            term /= (seq_t)m;                    /* integer division */

            sum += term;
        }
        cache[i] = sum;
    }
    cache_max = (cache_max < n) ? n : cache_max;
    return cache[n];
}
