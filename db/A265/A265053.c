/*
 * A265053  Poincaré series for hyperbolic reflection group with Coxeter
 *          diagram shown in Comments.
 * https://oeis.org/A265053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 38, u64 n <= 79, u128 n <= 161,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A265053: Poincaré series for a hyperbolic reflection group
 *
 * Generating function:
 *   \frac{(1+x)^3(1-x+x^2)(1+x^2)(1-x+x^2-x^3+x^4)}
 *        {(1-x)(1-2x+x^2-2x^4+2x^5-2x^6+x^8-2x^9+x^{10})}
 *
 * Recurrence (for n >= 12):
 *   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + 2*a(n-4) - 4*a(n-5)
 *          + 4*a(n-6) - 2*a(n-7) - a(n-8) + 3*a(n-9) - 3*a(n-10) + a(n-11)
 * For n < 12 the values are obtained from the constant terms of the numerator
 * and the initial part of the recurrence.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

/* Coefficients of the numerator (P(x)) for powers 0..11 */
static const seq_t P[12] = {
    (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)1,
    (seq_t)2, (seq_t)2, (seq_t)1, (seq_t)2, (seq_t)1,
    (seq_t)1, (seq_t)1
};

/* Coefficients of the denominator Q(x) = 1 -3x + 3x^2 - x^3 -2x^4 + 4x^5
 * -4x^6 + 2x^7 + x^8 -3x^9 + 3x^10 - x^11  (q[0] is unused) */
static const int q[12] = {
    0,  -3,   3,  -1,  -2,   4,  -4,   2,   1,  -3,   3,  -1
};

/* Coefficients of the recurrence for n >= 12 (coeff = -q[k]) */
static const int coeff[12] = {
    0,   3,  -3,   1,   2,  -4,   4,  -2,  -1,   3,  -3,   1
};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (n + 1 > cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure – should not happen */
        cache = tmp;

        for (idx_t i = cache_len; i <= n; ++i) {
            if (i <= 11) {
                /* Compute a[i] from the numerator coefficients and
                   the recurrence up to order i */
                seq_t val = P[i];
                for (int k = 1; k <= i; ++k) {
                    int qk = q[k];
                    if (qk >= 0) {
                        val -= mul_small(cache[i - k], (unsigned int)qk);
                    } else {
                        val += mul_small(cache[i - k], (unsigned int)(-qk));
                    }
                }
                cache[i] = val;
            } else {
                /* Recurrence for n >= 12 */
                seq_t pos_sum = 0;
                seq_t neg_sum = 0;
                for (int k = 1; k <= 11; ++k) {
                    int ck = coeff[k];
                    seq_t term = mul_small(cache[i - k], (unsigned int)(ck > 0 ? ck : -ck));
                    if (ck > 0) {
                        pos_sum += term;
                    } else {
                        neg_sum += term;
                    }
                }
                cache[i] = pos_sum - neg_sum;
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
