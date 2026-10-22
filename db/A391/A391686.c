/*
 * A391686  Antidiagonal sums of the array A391685.
 * https://oeis.org/A391686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*2^(2*n+3) - 21*n - 31)/9
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* exponent = 2*n + 3 */
    seq_t exp = ((seq_t)n) * 2 + 3;

    /* 2^(2*n+3) */
    seq_t pow2 = big_pow((seq_t)2, exp);

    /* numerator = 5*pow2 - 21*n - 31 */
    seq_t numerator = pow2 * 5;
    numerator -= ((seq_t)21) * (seq_t)n + 31;

    /* exact division by 9 */
    return numerator / 9;
}
