/*
 * A305065  a(n) = 48*2^n - 24.
 * https://oeis.org/A305065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 48*2^n - 24
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Compute 2^n */
    seq_t two_pow = seq_pow((seq_t)2, n);

    /* a(n) = 48*2^n - 24 */
    seq_t result = two_pow * (seq_t)48;
    result -= (seq_t)24;

    return result;
}
