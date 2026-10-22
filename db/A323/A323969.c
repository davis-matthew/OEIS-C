/*
 * A323969  Number of 5 X n integer matrices (m_{i,j}) such that m_{1,1}=0,
 *          m_{5,n}=2, and all rows, columns, and falling diagonals are
 *          (weakly) monotonic without jumps of 2.
 * https://oeis.org/A323969
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{j=0}^{min(n,11)} p_j * C(n - j + 10, 10) where p_j are
 *            the coefficients of the numerator of the OGF: Q(x) = 3x^11 -
 *            29x^10 + 125x^9 - 314x^8 + 501x^7 - 517x^6 + 323x^5 - 84x^4 -
 *            20x^3 + 30x^2 - 5x + 1 and C(a,b) is the binomial coefficient.
 *            The sequence thus grows like a polynomial of d...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 27, u64 n <= 290,
 *            u128 n <= 10000, big any n
 * Helpers:   binom_fixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom_fixed.h"

seq_t a(idx_t n)
{
    /* signed coefficients p_j */
    static const int coeff_signed[12] = {
        1,   /* p0 */
        -5,  /* p1 */
        30,  /* p2 */
        -20, /* p3 */
        -84, /* p4 */
        323, /* p5 */
        -517,/* p6 */
        501, /* p7 */
        -314,/* p8 */
        125, /* p9 */
        -29, /* p10 */
        3    /* p11 */
    };
    /* positive absolute values of coefficients to avoid unsigned overflow */
    static const seq_t coeff_abs[12] = {
        1U, 5U, 30U, 20U, 84U, 323U, 517U, 501U, 314U, 125U, 29U, 3U
    };

    seq_t pos = 0; /* sum of positive terms */
    seq_t neg = 0; /* sum of absolute values of negative terms */

    idx_t max_j = n < 11 ? n : 11;
    for (idx_t j = 0; j <= max_j; ++j) {
        /* compute binom(n - j + 10, 10) */
        idx_t arg = n - j + 10;
        seq_t term = binom_fixed(arg, 10);

        if (coeff_signed[j] >= 0) {
            pos += (seq_t)coeff_signed[j] * term;
        } else {
            neg += coeff_abs[j] * term;
        }
    }

    /* final result: pos - neg; sequence is always non‑negative */
    return pos - neg;
}
