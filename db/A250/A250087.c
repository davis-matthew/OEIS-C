/*
 * A250087  Number of length 6+5 0..n arrays with every six consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining four terms.
 * https://oeis.org/A250087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = P(n)/504 where P(n) = 170*n^9 + 4335*n^8 + 24984*n^7 +
 *            60648*n^6 + 79506*n^5 + 79275*n^4 + 76816*n^3 + 50286*n^2 +
 *            13572*n + 504
 * Range:     u8 none, u16 n <= 2, u32 n <= 11, u64 n <= 153, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* defined for n ≥ 1 only */
    /* Coefficients of P(n) (integer polynomial after removing fraction) */
    static const unsigned long long coeffs[10] = {
        170,      4335,      24984,   60648,   79506,
        79275,    76816,     50286,   13572,   504
    };

    seq_t N = (seq_t)n;
    unsigned __int128 P = 0;
    unsigned __int128 NN = (unsigned __int128)N;

    /* Horner's rule for efficient polynomial evaluation */
    for (int i = 0; i < 10; ++i) {
        P = P * NN + coeffs[i];
    }

    /* Divide by 504 (common denominator of the original rational polynomial) */
    return (seq_t)(P / 504u);
}
