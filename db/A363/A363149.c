/*
 * A363149  Expansion of 1 / Sum_{k>=0} x^(k*(5*k - 3)/2).
 * https://oeis.org/A363149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 25, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A363149 – Expansion of 1 / Sum_{k>=0} x^(k*(5*k-3)/2).
 *
 * Formula:
 *   a(0) = 1
 *   For n > 0:
 *     a(n) = - Σ_{k>=1, e_k ≤ n} a(n – e_k)
 *   where e_k = k*(5*k-3)/2  (the exponents of the denominator).
 */

#include <stddef.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;   /* number of terms stored (max index + 1) */

    /* static cache for exponents e_k = k*(5*k-3)/2  (k >= 1) */
    static idx_t *exps = NULL;
    static size_t exps_cap = 0;
    static size_t exps_len = 0;
    static idx_t next_k = 1;        /* next k to generate an exponent for */

    /* a(0) is always 1; make sure it is stored. */
    if (n == 0) {
        if (cache_len == 0) {
            ensure_capacity(1, &cache_cap, (void **)&cache, sizeof(seq_t));
            cache[0] = 1;
            cache_len = 1;
        }
        return cache[0];
    }

    /* Extend exponent list until e_k > n. */
    while (1) {
        idx_t k  = next_k;
        idx_t e  = (5 * k * k - 3 * k) / 2;   /* e_k = k*(5k-3)/2 */
        if (e > n) break;

        ensure_capacity(exps_len + 1, &exps_cap,
                        (void **)&exps, sizeof(idx_t));
        exps[exps_len++] = e;
        next_k++;
    }

    /* Extend the term cache if necessary. */
    if (cache_len <= (size_t)n) {
        ensure_capacity((size_t)n + 1, &cache_cap,
                        (void **)&cache, sizeof(seq_t));
        if (cache_len == 0) {
            cache[0] = 1;
            cache_len = 1;
        }

        /* Compute new terms from cache_len up to n. */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = 0;
            for (size_t j = 0; j < exps_len; ++j) {
                idx_t e = exps[j];
                if (e > i) break;
                sum += cache[(size_t)(i - e)];
            }
            cache[(size_t)i] = -sum;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
