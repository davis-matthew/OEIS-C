/*
 * A356185  The difference between number of even and number of odd
 *          Grassmannian permutations of size n.
 * https://oeis.org/A356185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^⌈n/2⌉ - n
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute ceil(n/2) */
    idx_t exp = (n + 1) >> 1;
    seq_t pow = pow2(exp);
    seq_t result = pow - (seq_t)n;
    return result;
}
