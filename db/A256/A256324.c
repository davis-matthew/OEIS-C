/*
 * A256324  a(n) = denominator of (1/n^3)*(-1/(n+1) + 16/(n+2) + 3/(4*(2*n+1))
 *          - 81/(4*(2*n+3))), term of a BBP-type series representation of
 *          zeta(3) by V. Adamchik and S. Wagon.
 * https://oeis.org/A256324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of (1/n^3)*(-1/(n+1) + 16/(n+2) +
 *            3/(4*(2*n+1)) - 81/(4*(2*n+3))) (offset 1)
 * Range:     u8 none, u16 n <= 2, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The denominator is computed from the reduced fraction
   N / (4*n^3*(n+1)*(n+2)*(2*n+1)*(2*n+3)) where
   N = 64*(n+1)*(2*n+1)*(2*n+3) + 3*(n+1)*(n+2)*(2*n+3)
       - 81*(n+1)*(n+2)*(2*n+1) - 4*(n+2)*(2*n+1)*(2*n+3). */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t to avoid overflow in multiplications */
    seq_t sn  = (seq_t)n;            /* n */
    seq_t sn1 = sn + (seq_t)1;       /* n+1 */
    seq_t sn2 = sn + (seq_t)2;       /* n+2 */
    seq_t two_n_plus1 = (seq_t)2 * sn + (seq_t)1;  /* 2*n+1 */
    seq_t two_n_plus3 = (seq_t)2 * sn + (seq_t)3;  /* 2*n+3 */

    /* Numerator components */
    seq_t N2 = (seq_t)64 * sn1 * two_n_plus1 * two_n_plus3;
    seq_t N3 = (seq_t)3  * sn1 * sn2 * two_n_plus3;
    seq_t N4 = (seq_t)81 * sn1 * sn2 * two_n_plus1;
    seq_t N1 = (seq_t)4  * sn2 * two_n_plus1 * two_n_plus3;

    /* Compute the numerator N = N2 + N3 - N4 - N1 */
    seq_t N = N2 + N3;
    N = N - N4;
    N = N - N1;

    /* Denominator D = 4 * n^3 * (n+1)*(n+2)*(2*n+1)*(2*n+3) */
    seq_t sn_sq  = sn * sn;          /* n^2 */
    seq_t sn_cub = sn_sq * sn;       /* n^3 */
    seq_t D = (seq_t)4 * sn_cub * sn1 * sn2 * two_n_plus1 * two_n_plus3;

    /* Reduce fraction and return denominator */
    seq_t g = gcd(D, N);
    return D / g;
}
