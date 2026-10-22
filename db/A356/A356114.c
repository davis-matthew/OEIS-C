/*
 * A356114  Number of irreducible permutations of n with partition type [2, 1,
 *          1, ..., 1] (with '1' taken n - 2 times).
 * https://oeis.org/A356114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - n - 3 for n >= 3; a(0)=0, a(1)=0, a(2)=0
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;
    seq_t val = pow2(n);
    val -= n;
    val -= 3;
    return val;
}
