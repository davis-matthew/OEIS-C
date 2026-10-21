/*
 * A253103  A001045(n)^3.
 * https://oeis.org/A253103
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((2^n - (-1)^n)/3)^3 = A001045(n)^3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* compute 2^n */
    seq_t pow2_n = pow2(n);

    /* compute A001045(n) = (2^n - (-1)^n)/3 */
    seq_t x;
    if ((n & 1) == 0)            /* even n: (-1)^n = 1   */
        x = (pow2_n - (seq_t)1) / 3;
    else                        /* odd n: (-1)^n = -1  */
        x = (pow2_n + (seq_t)1) / 3;

    /* return x^3 */
    seq_t sq = seq_mul(x, x);
    return seq_mul(sq, x);
}
