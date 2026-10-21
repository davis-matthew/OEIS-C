/*
 * A270471  Expansion of g.f. (1-3*x)/(1-7*x).
 * https://oeis.org/A270471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = 4 * 7^(n-1) for n >= 1 (equivalently
 *            a(n)=7*a(n-1) for n>1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t p = pow_uint(7, n - 1);
    return p * (seq_t)4;
}
