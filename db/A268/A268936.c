/*
 * A268936  Number of sequences in {1,...,n}^n with longest increasing
 *          subsequence of length n-2.
 * https://oeis.org/A268936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{j=0}^6 q_j * C(n+3 - j, 6) * where q = {10, 105, 116,
 *            -91, 57, -20, 3} * a(0) = a(1) = a(2) = 0 * See the rational
 *            generating function:
 *            x^3*(3*x^6-20*x^5+57*x^4-91*x^3+116*x^2+105*x+10)/(1-x)^7
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 51, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* For n < 3 the sequence is defined to be 0. */
    if (n < 3) {
        return 0;
    }

    /* Coefficients of the polynomial.  The array is signed to
       easily distinguish positive and negative coefficients.  */
    const int coeffs[7] = {10, 105, 116, -91, 57, -20, 3};

    seq_t pos_total = 0;   /* Sum of positive contributions */
    seq_t neg_total = 0;   /* Sum of absolute values of negative contributions */

    for (idx_t j = 0; j < 7; ++j) {
        /* Compute the binomial coefficient C(n + 3 - j, 6).
           For n < 3 this loop is skipped, so no negative arguments are
           passed to binom.  binom is expected to return 0 if the
           upper index is smaller than 6. */
        seq_t comb = binom(n + 3 - j, 6);

        if (coeffs[j] > 0) {
            pos_total += comb * (seq_t)coeffs[j];
        } else if (coeffs[j] < 0) {
            neg_total += comb * (seq_t)(-coeffs[j]);
        }
        /* If coeff is 0 nothing is added. */
    }

    /* We guarantee that pos_total >= neg_total because the final
       result of the sequence is always non‑negative. */
    return pos_total - neg_total;
}
