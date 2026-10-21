/*
 * A250086  Number of length 5+5 0..n arrays with every six consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining four terms.
 * https://oeis.org/A250086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 13, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (1/63)*n^9 + (121/84)*n^8 + (108/7)*n^7 + (166/3)*n^6 +
 *              (469/5)*n^5 + (1199/12)*n^4 + (5753/63)*n^3 + (2659/42)*n^2 +
 *              (2146/105)*n + 1
 *
 * This implementation evaluates the equivalent integer polynomial
 * 1260*a(n) = 20*n^9 + 1815*n^8 + 19440*n^7 + 69720*n^6 + 118188*n^5
 *            + 125895*n^4 + 115060*n^3 + 79770*n^2 + 25752*n + 1260
 * and then divides by 1260.  All intermediate calculations are performed
 * in unsigned __int128 to avoid overflow for all supported n values.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* coefficients of 1260*a(n) in descending powers of n */
    static const uint64_t coeff[10] = {
        20ULL, 1815ULL, 19440ULL, 69720ULL,
        118188ULL, 125895ULL, 115060ULL, 79770ULL,
        25752ULL, 1260ULL
    };

    unsigned __int128 num  = coeff[0];
    unsigned __int128 sn   = (unsigned __int128)n;

    for (idx_t i = 1; i < 10; ++i)
        num = num * sn + coeff[i];

    unsigned __int128 res = num / (unsigned __int128)1260;

    return (seq_t)res;
}
