/*
 * A252926  Number of n X 4 nonnegative integer arrays with upper left 0 and
 *          lower right n+4-6 and value increasing by 0 or 1 with every step
 *          right or down.
 * https://oeis.org/A252926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0}^{14} r_j * binom(n + 14 - j, 16)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 15, u64 n <= 73, u128 n <= 210,
 *            big any n
 * Helpers:   binom_16
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where r = [+1, +16, +190, +350, -1419, +3792, -7860, +11238, -11247, +8133, -4280, +1600, -401, +60, -4] */
/* This follows from the generating function G(x) = x^2*(-1-16x-190x^2-350x^3+1419x^4-3792x^5+7860x^6-11238x^7+11247x^8-8133x^9+4280x^10-1600x^11+401x^12-60x^13+4x^14)/(x-1)^17. */

#include <stdint.h>
#include "../utils/binom_16.h"

/* Integer coefficients for the numerator of the generating function.  They
   already contain the overall minus sign of (x-1)^17. */
static const int64_t R[15] = {
    1,    16,   190,   350, -1419,
   3792, -7860, 11238, -11247,  8133,
  -4280,  1600, -401,    60,   -4
};

seq_t a(idx_t n)
{
    seq_t pos = 0;   /* sum of positive contributions */
    seq_t neg = 0;   /* sum of negative contributions (stored abs value) */

    for (int j = 0; j < 15; ++j) {
        idx_t k = n + 14 - j;          /* n + 14 - j = n + (15 - (j+1)) */
        seq_t bw = binom_16(k);
        if (bw == 0) continue;

        int64_t coeff = R[j];
        if (coeff >= 0) {
            pos += bw * (seq_t)coeff;
        } else {
            neg += bw * (seq_t)(-coeff);
        }
    }

    /* Final result: pos - neg (pos > neg for all n >= 1). */
    seq_t result = pos;
    result -= neg;
    return result;
}
