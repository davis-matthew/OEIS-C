/*
 * A351934  Expansion of e.g.f. exp(x / (1 - x^3/6)).
 * https://oeis.org/A351934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 23, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0)=1; a(1)=1; a(2)=1; a(3)=1;
 * for n>3: a(n)=Σ_{k=0..⌊(n-1)/3⌋}
 *     ((3*k+1)!/6^k) * C(n-1,3*k) * a(n-1-3*k)
 * (C denotes binomial coefficient)
 */

#include <stdlib.h>
#include <string.h>

/* required external helpers */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* static storage for memoization */
    static seq_t *cache = NULL;   /* array of computed a[...] */
    static size_t cache_size = 0; /* number of valid entries in cache */
    static size_t cache_cap  = 0; /* current allocation size */

    /* static storage for precomputed coefficients ((3*k+1)!/6^k) */
    static seq_t *coeffs   = NULL;
    static size_t coeff_len = 0; /* number of valid coefficients (0..coeff_len-1) */
    static size_t coeff_cap = 0; /* current allocation size */

    /* helper to extend the coefficient array to at least `maxk+1` entries */
    if (coeff_len <= 0) {
        /* allocate initial capacity */
        coeff_cap = 64;
        coeffs = (seq_t *)malloc(coeff_cap * sizeof(seq_t));
        coeffs[0] = (seq_t)1;            /* k=0: 1! / 6^0 = 1 */
        coeff_len = 1;
    }
    idx_t need_coeff = (n + 2) / 3; /* floor((n-1)/3)+1 for safety when n>=3 */
    if (need_coeff > coeff_len) {
        if (need_coeff > coeff_cap) {
            size_t new_cap = coeff_cap * 2;
            if (new_cap < need_coeff) new_cap = need_coeff;
            coeffs = (seq_t *)realloc(coeffs, new_cap * sizeof(seq_t));
            coeff_cap = new_cap;
        }
        for (idx_t k = coeff_len; k < need_coeff; ++k) {
            /* compute coeff[k] = coeff[k-1] * (3*k+1)*(3*k)*(3*k-1) / 6 */
            seq_t tmp = coeffs[k - 1];
            tmp = tmp * (seq_t)(3 * k + 1);
            tmp = tmp * (seq_t)(3 * k);
            tmp = tmp * (seq_t)(3 * k - 1);
            tmp = tmp / (seq_t)6;
            coeffs[k] = tmp;
        }
        coeff_len = need_coeff;
    }

    /* helper to extend the cache up to `n` */
    if (cache_size <= n) {
        if (cache_size < 4) {
            cache_cap = 16;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            for (idx_t i = 0; i < 4; ++i) cache[i] = (seq_t)1;
            cache_size = 4;
        }
        if (n + 1 > cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < n + 1) new_cap = n + 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            idx_t maxk = (i - 1) / 3;
            for (idx_t k = 0; k <= maxk; ++k) {
                seq_t mult = coeffs[k];
                mult = mult * binomial(i - 1, 3 * k);
                mult = mult * cache[i - 1 - 3 * k];
                sum = sum + mult;
            }
            cache[i] = sum;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
