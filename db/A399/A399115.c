/*
 * A399115  Expansion of e.g.f. 1/(cosh(x) - x*cosh(x)^2).
 * https://oeis.org/A399115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(n,k) * a(k) * b(n-k), b(1)=1,
 *            b(odd>1)=n*2^{n-2}, b(even)=-1. a(0)=1. Implementation uses
 *            memoisation
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   mul_small, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    /* static cache of computed a values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cached_upto = 0;

    /* static cache for powers of two up to current n-1 */
    static seq_t *pow2_cache = NULL;
    static idx_t pow2_cap = 0;

    /* expand cache if needed */
    if (cache == NULL || cache_cap <= n) {
        size_t new_cap = (size_t)n + 1;          /* need indices 0..n */
        if (cache == NULL) {
            cache = seq_alloc(new_cap);
        } else {
            cache = seq_realloc(cache, cache_cap, new_cap);
        }
        cache_cap = new_cap;
    }

    /* Expand powers-of-two cache if needed */
    if (pow2_cache == NULL || pow2_cap <= n-1) {
        size_t new_cap = (n>=1) ? (size_t)n : 1;  /* need indices 0..n-2, so size n-1, but guard */
        if (pow2_cache == NULL) {
            pow2_cache = seq_alloc(new_cap);
        } else {
            pow2_cache = seq_realloc(pow2_cache, pow2_cap, new_cap);
        }
        /* compute new entries */
        for (idx_t i = pow2_cap; i < new_cap; ++i) {
            if (i == 0)
                pow2_cache[i] = (seq_t)1;
            else
                pow2_cache[i] = mul_small(pow2_cache[i-1], 2U);
        }
        pow2_cap = new_cap;
    }

    /* initialise a[0] if not already done */
    if (cached_upto < 0) cached_upto = 0;   /* dummy to silence unused warning */
    if (cached_upto == 0 && cache[0] == (seq_t)0) {
        cache[0] = (seq_t)1;
    }

    /* compute values up to n */
    for (idx_t cur = cached_upto + 1; cur <= n; ++cur) {
        seq_t S_odd = 0;    /* sum of terms with positive contribution */
        seq_t sum_even = 0; /* sum of even j terms (will be subtracted) */

        seq_t binom = (seq_t)1; /* binom(cur,0) */
        for (idx_t k = 0; k < cur; ++k) {
            seq_t a_k = cache[k];
            idx_t j = cur - k;                  /* we need b(j) */
            seq_t prod = binom * a_k;           /* binom * a[k] */

            if (j == 1) {
                /* b(1)=1 */
                S_odd += prod;
            } else if ((j & 1ULL) == 0) {
                /* even j, b(j) = -1 -> subtract later */
                sum_even += prod;
            } else {
                /* odd j >=3: b(j) = j * 2^{j-2} */
                prod = prod * (seq_t)j;                 /* multiply by j */
                prod = prod * pow2_cache[j-2];          /* multiply by 2^{j-2} */
                S_odd += prod;
            }

            /* update binomial for next k */
            if (k + 1 < cur) {
                seq_t mult_factor = (seq_t)(cur - k);
                seq_t denom      = (seq_t)(k + 1);
                binom = (binom * mult_factor) / denom;
            }
        }

        /* a[cur] = S_odd - sum_even (guaranteed non‑negative) */
        cache[cur] = S_odd - sum_even;
    }

    cached_upto = n;
    return cache[n];
}
