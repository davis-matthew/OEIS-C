/*
 * A305064  a(n) = 42*2^n - 20.
 * https://oeis.org/A305064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 42*2^n - 20
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^n */
    seq_t val = pow2(n);

    /* Multiply by 42 */
    val *= (seq_t)42;

    /* Subtract 20 */
    val -= (seq_t)20;

    return val;
}
