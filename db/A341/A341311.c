/*
 * A341311  G.f. =
 *          (1+x^2+2*x^3+3*x^4+4*x^5+3*x^6+4*x^7+3*x^8+2*x^9+x^10)/(1-x^6)^2.
 * https://oeis.org/A341311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0}^{10} c[i] * (k+1) where k = (n-i)/6, if n ≥ i and
 *            (n-i) mod 6 = 0, with coefficients c = {1,0,1,2,3,4,3,4,3,2,1}
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Pre‑defined coefficients of the numerator polynomial */
    static const unsigned char coef[11] =
        {1, 0, 1, 2, 3, 4, 3, 4, 3, 2, 1};

    seq_t result = 0;
    idx_t i;

    for (i = 0; i < 11; ++i) {
        if (n >= i) {
            idx_t diff = n - i;
            if (diff % 6 == 0) {
                seq_t k = ((seq_t)(diff / 6)) + 1;          /* (k+1) */
                result += k * (seq_t)coef[i];
            }
        }
    }
    return result;
}
