/*
 * A305164  a(n) = 28*2^n - 22.
 * https://oeis.org/A305164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 28*2^n - 22
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^n using the utility function to support arbitrary-precision */
    seq_t two_pow = pow2(n);

    /* Apply the formula: a(n) = 28*2^n - 22 */
    seq_t result = (seq_t)28 * two_pow - (seq_t)22;

    return result;
}
