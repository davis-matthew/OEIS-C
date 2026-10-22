/*
 * A380029  Expansion of e.g.f. (1 - 3*x*exp(x))^(1/3).
 * https://oeis.org/A380029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n! * Sum_{k=0..n} (-3)^k * k^(n-k) *
 *            binomial(1/3,k)/(n-k)!
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 9, i64 n <= 15, i128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"


/* Return the n‑th term of OEIS sequence A380029; a(0)=1. */
seq_t a(idx_t n)
{
    seq_t sum  = 0;           /* running total */
    seq_t binom = 1;          /* C(n,0) = 1 */
    seq_t prod = 1;           /* product_{j=0}^{k-1} (1-3*j) (empty product = 1) */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t power;
        if (k == 0) {
            /* k^0 = 1, 0^(positive)=0 */
            power = (n == 0) ? (seq_t)1 : (seq_t)0;
        } else {
            power = big_pow((seq_t)k, (seq_t)(n - k));
        }

        /* term = C(n,k) * (-1)^k * prod * k^(n-k) */
        seq_t term = binom;
        if (k & 1) {
            term = -term;
        }
        term *= prod;
        term *= power;

        sum += term;

        if (k < n) {
            /* update binomial to C(n,k+1) */
            binom = binom * (seq_t)(n - k);
            binom = binom / (seq_t)(k + 1);

            /* update product to prod_{j=0}^{k} (1-3*j) for next k */
            prod = prod * ((seq_t)1 - (seq_t)(3 * k));
        }
    }

    return sum;
}
