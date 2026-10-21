/*
 * A273348  The sum of the semiperimeters of the bargraphs of area n (n>=1).
 * https://oeis.org/A273348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (15*n*2^n+29*2^n-2*(-1)^n-18)/36
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 23, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t two_pow_n = pow2(n);                 /* 2^n */
    seq_t term1 = ((seq_t)15) * ((seq_t)n * two_pow_n); /* 15*n*2^n */
    seq_t term2 = ((seq_t)29) * two_pow_n;          /* 29*2^n */
    seq_t numerator = term1 + term2;               /* 15*n*2^n + 29*2^n */

    /* add or subtract 2 depending on parity of n */
    if ((n & 1) == 0)
        numerator -= 2;   /* even n: -2 */
    else
        numerator += 2;   /* odd n: +2 */

    numerator -= 18;       /* subtract constant 18 */

    return numerator / 36; /* divide by 36 */
}
