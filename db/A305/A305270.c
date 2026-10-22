/*
 * A305270  a(n) = 140*2^n - 112.
 * https://oeis.org/A305270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 140*2^n - 112
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 24, u64 n <= 56, u128 n <= 120,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^n as a seq_t */
    seq_t two_pow = pow2(n);

    /* a(n) = 140*2^n - 112 */
    return (seq_t)140 * two_pow - (seq_t)112;
}
