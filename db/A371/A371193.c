/*
 * A371193  a(n) = 2^(4*n-4)*(2^n-1).
 * https://oeis.org/A371193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(4*n-4)*(2^n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0;  /* a(0) = 0 */

    idx_t exp = 4u * n - 4u;          /* 4*n - 4  (n >= 1) */
    seq_t part1 = pow2(exp);          /* 2^(4*n-4) */
    seq_t part2 = pow2(n) - (seq_t)1; /* 2^n - 1      */
    return part1 * part2;             /* a(n)          */
}
