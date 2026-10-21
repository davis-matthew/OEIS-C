/*
 * A261383  Number of 5-ascent sequences of length n that avoid the pattern
 *          00.
 * https://oeis.org/A261383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (−(5*x^14 −56*x^13 +293*x^12 −959*x^11 +2212*x^10 *
 *            −3815*x^9 +5055*x^8 −5190*x^7 +4127*x^6 −2528*x^5 +1175*x^4 *
 *            −400*x^3 +95*x^2 −14*x +1))/(x−1)^15 * * The coefficients of the
 *            numerator polynomial are applied as * a linear combination of
 *            binomial(n‑i+14, 14) for i=0…14. Binomial values...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 24, u64 n <= 138, u128 n <= 1000,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Compute binomial(n,14).  Assumes n ≥ 14.  Uses reduction by
 *  the fixed denominator 14! to keep intermediate values small. */
static seq_t binom_n_14(idx_t n)
{
    if (n < 14) return 0;

    /* numerators: n, n‑1, … , n‑13 */
    idx_t num[14];
    for (idx_t i = 0; i < 14; ++i) num[i] = n - i;

    /* denominators: 2,3,…,14 (1 is omitted) */
    idx_t den[13];
    for (idx_t j = 0; j < 13; ++j) den[j] = j + 2;

    /* Reduce the fractions by cancellation of common factors with
     * the denominators.  Since the denominators are small the
     * algorithm is fast. */
    for (idx_t j = 0; j < 13; ++j) {
        if (den[j] == 1) continue;
        for (idx_t i = 0; i < 14; ++i) {
            if (num[i] == 1) continue;
            seq_t g = gcd((seq_t)num[i], (seq_t)den[j]);
            if (g > 1) {
                num[i] /= (idx_t)g;
                den[j] /= (idx_t)g;
                if (den[j] == 1) break;
            }
        }
    }

    /* After cancellation all remaining denominators must be 1. */
    seq_t res = 1;
    for (idx_t i = 0; i < 14; ++i) {
        res *= (seq_t)num[i];
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Coefficients of the numerator polynomial in descending order
     * of x^14 down to x^0, i.e. for i = 0 it is the coefficient of
     * x^14, for i = 1 the coefficient of x^13, … , i = 14 the
     * constant term. */
    static const int coeff[15] = {
        1, -14, 95, -400, 1175, -2528, 4127, -5190, 5055,
        -3815, 2212, -959, 293, -56, 5
    };

    seq_t pos = 0;   /* sum of unsigned contributions */
    seq_t neg = 0;   /* sum of magnitudes of negative contributions */

    for (idx_t i = 0; i < 15; ++i) {
        /* When n < i the term C(n‑i+14, 14) is zero (negative
         * index would underflow).  Skip such cases. */
        if (n < i) continue;

        idx_t N = n - i + 14;          /* N ≥ 14 for all considered i */
        seq_t term = binom_n_14(N);

        if (coeff[i] >= 0) {
            pos += (seq_t)coeff[i] * term;
        } else {
            neg += (seq_t)(-coeff[i]) * term;
        }
    }

    /* The combination guarantees that pos ≥ neg. */
    return pos - neg;
}
