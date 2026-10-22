/*
 * A351792  Expansion of e.g.f. 1/(1 - x*exp(-3*x)).
 * https://oeis.org/A351792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n * Sum_{k=0..n-1} (-3)^{n-1-k} * binomial(n-1,k) * a(k)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 10, i64 n <= 17, i128 n <= 19,
 *            big any n
 * Helpers:   expand
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/expand.h"

seq_t a(idx_t n)
{
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;   /* indices 0 .. cache_len-1 are valid */

    static seq_t *pow3      = NULL;
    static size_t pow3_cap  = 0;
    static size_t pow3_len  = 0;   /* elements 0 .. pow3_len-1 are valid */

    size_t idx = (size_t)n;

    /* ----- Pre‑compute powers of -3 up to idx-1 ----- */
    if (idx > 0) {
        size_t need = idx;              /* we need indices 0 .. idx-1 */
        expand(&pow3_cap, &pow3, need);
        if (pow3_len == 0) {
            pow3[0] = (seq_t)1;
            pow3_len = 1;
        }
        for (size_t i = pow3_len; i < need; ++i)
            pow3[i] = pow3[i-1] * (seq_t)(-3);
        pow3_len = need;
    } else {
        /* idx == 0 */
        if (pow3_len == 0) {
            expand(&pow3_cap, &pow3, 1);
            pow3[0] = (seq_t)1;
            pow3_len = 1;
        }
    }

    /* ----- Ensure cache can hold the required index ----- */
    expand(&cache_cap, &cache, idx + 1);
    if (cache_len == 0) {
        cache[0] = (seq_t)1;           /* a(0) = 1 */
        cache_len = 1;
    }

    /* ----- Compute any missing terms up to idx ----- */
    for (size_t cur = cache_len; cur <= idx; ++cur) {
        seq_t sum   = 0;
        seq_t binom = 1;                /* binomial(cur-1,0) */

        for (size_t k = 0; k < cur; ++k) {
            seq_t p    = pow3[cur - 1 - k];
            seq_t term = binom;
            term      *= p;
            term      *= cache[k];
            sum       += term;

            if (k + 1 < cur) {         /* update to binomial(cur-1,k+1) */
                binom = (binom * (seq_t)(cur - 1 - k)) / (seq_t)(k + 1);
            }
        }
        cache[cur] = (seq_t)(cur) * sum;
    }

    cache_len = idx + 1;
    return cache[idx];
}
