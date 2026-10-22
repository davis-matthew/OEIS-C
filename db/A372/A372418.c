/*
 * A372418  Expansion of (1/x) * Series_Reversion( x * (1-x)^2 / (1-x+x^3)^2
 *          ).
 * https://oeis.org/A372418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/3⌋} binomial(2n+2, k) *
 *            binomial(n-2k-1, n-3k)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static seq_t binomial_cm(idx_t n, idx_t k) {
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;    /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;

        /* reduce by gcd to keep numbers small */
        seq_t g = gcd(num, den);
        num /= g;
        den /= g;

        g = gcd(res, den);
        res /= g;
        den /= g;

        /* den should be 1 now, but just in case */
        if (den != (seq_t)1) {
            /* final division to ensure exactness */
            res *= num;
            res /= den;
        } else {
            res *= num;
        }
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)1;

    seq_t sum = (seq_t)0;
    idx_t k_max = n / (idx_t)3;

    for (idx_t k = (idx_t)0; k <= k_max; ++k) {
        /* binomial(2n+2, k) */
        seq_t c1 = binomial_cm((idx_t)(2*n + (idx_t)2), k);

        /* compute binomial(n-2k-1, n-3k) carefully */
        idx_t n_minus_2k_minus_1 = n - (idx_t)2*k - (idx_t)1;
        idx_t n_minus_3k = n - (idx_t)3*k;
        seq_t c2 = (seq_t)0;

        if (n >= (idx_t)2*k + (idx_t)1 && n_minus_3k <= n_minus_2k_minus_1) {
            c2 = binomial_cm(n_minus_2k_minus_1, n_minus_3k);
        }

        sum += c1 * c2;
    }

    return sum / (seq_t)(n + (idx_t)1);   /* division is exact */
}
