/*
 * A364112  Expansion of e.g.f. 3*x/(exp(-3*x)+exp(-x)+exp(x)).
 * https://oeis.org/A364112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = series coefficient of the EGF
 *            3*x/(exp(-3*x)+exp(-x)+exp(x))
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, for n≥1: 
 *   a(n) = ( δ_{n,1}*3*n! - Σ_{k=0}^{n-1} C(n,k)*a(k)*B(n-k) ) / 3
 * where B(m) = (-3)^m + (-1)^m + 1
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;             /* number of terms currently saved */

    /* static cache for B(m) values */
    static seq_t *B = NULL;
    static size_t B_cap = 0;

    /* ensure capacity for cache and B arrays up to n */
    if ((idx_t)(cache_cap) < n + 1) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_arr) return 0; /* allocation failure, return 0 */
        cache = new_arr;
        cache_cap = new_cap;
    }
    if ((idx_t)(B_cap) < n + 1) {
        size_t new_cap = B_cap ? B_cap * 2 : 1;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *new_arr = (seq_t *)realloc(B, new_cap * sizeof(seq_t));
        if (!new_arr) return 0;
        B = new_arr;
        B_cap = new_cap;
    }

    /* compute B(m) for m = 0 .. n */
    seq_t pow_minus3 = 1;
    seq_t pow_minus1 = 1;
    B[0] = pow_minus3 + pow_minus1 + (seq_t)1; /* B(0)=3 */
    for (idx_t m = 1; m <= n; ++m) {
        pow_minus3 = pow_minus3 * (seq_t)(-3);
        pow_minus1 = pow_minus1 * (seq_t)(-1);
        B[m] = pow_minus3 + pow_minus1 + (seq_t)1;
    }

    /* ensure a(0) is set */
    if (cache_len == 0) {
        cache[0] = 0;
        cache_len = 1;
    }

    /* compute missing terms up to n */
    for (idx_t cur = cache_len; cur <= n; ++cur) {
        seq_t sum = 0;
        seq_t binom = 1; /* C(cur,0) */
        for (idx_t k = 0; k < cur; ++k) {
            /* term = C(cur,k) * a(k) * B(cur-k) */
            seq_t term = binom * cache[k];
            term *= B[cur - k];
            sum += term;

            if (k + 1 < cur) {
                /* update binomial coefficient to C(cur,k+1) */
                binom = binom * (seq_t)(cur - k) / (seq_t)(k + 1);
            }
        }
        seq_t tmp;
        if (cur == 1) {
            tmp = (seq_t)3 - sum;
        } else {
            tmp = -sum;
        }
        cache[cur] = tmp / (seq_t)3;
    }

    cache_len = n + 1;
    return cache[n];
}
