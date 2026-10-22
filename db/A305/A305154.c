/*
 * A305154  a(n) = 36*2^n + 9.
 * https://oeis.org/A305154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 36*2^n + 9
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute 2^n */
    seq_t val = pow2(n);

    /* multiply by 36 and add 9 */
    val = val * (seq_t)36;
    val += (seq_t)9;

    return val;
}
