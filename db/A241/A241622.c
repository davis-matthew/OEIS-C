/*
 * A241622  Number of length 7+2 0..n arrays with no consecutive three
 *          elements summing to more than n.
 * https://oeis.org/A241622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0}^{9} c_j * binom(n+8-j, 9) with c = [41, 146,
 *            460, -19, 283, -209, 120, -45, 10, -1] This follows from the
 *            G.f. x*(41+146x+460x^2-19x^3+283x^4-209x^5
 *            +120x^6-45x^7+10x^8-x^9)/(1-x)^10
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 21, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Coefficients of the numerator polynomial.  Negative values
       are handled by subtraction. */
    static const int coeff[10] = {
        41,   146,   460,   -19,   283,
       -209,   120,   -45,    10,   -1
    };

    seq_t result = 0;
    for (int j = 0; j < 10; ++j) {
        idx_t top = n + 8 - (idx_t)j;          // n+8-j
        if (top < 9) continue;                // binom(a,b)=0 if a<b
        seq_t bin = binom(top, (idx_t)9);

        int c = coeff[j];
        if (c >= 0)
            result += (seq_t)c * bin;
        else
            result -= (seq_t)(-c) * bin;
    }
    return result;
}
