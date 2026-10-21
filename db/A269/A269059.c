/*
 * A269059  Construct a hollow square of 1's of side n and fill its interior
 *          with 0's to create a stack of n binary numbers. Express the sum of
 *          the stack in decimal.
 * https://oeis.org/A269059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2*(2^n-1)+(n-2)*(2^(n-1)+1)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t two_pow_n  = pow2(n);
    seq_t two_pow_n1 = pow2(n - 1);

    seq_t part1 = ((seq_t)2) * (two_pow_n - (seq_t)1);
    seq_t part2 = ((seq_t)(n - 2)) * (two_pow_n1 + (seq_t)1);

    return part1 + part2;
}
