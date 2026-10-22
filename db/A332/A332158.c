/*
 * A332158  a(n) = 5*(10^(2*n+1)-1)/9 + 3*10^n.
 * https://oeis.org/A332158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*(10^(2*n+1)-1)/9 + 3*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* 10^n */
    seq_t pow10_n = pow_uint(10, n);

    /* 10^(2*n+1) */
    idx_t exp2n1 = n * 2 + 1;
    seq_t pow10_2n1 = pow_uint(10, exp2n1);

    /* 5*(10^(2*n+1)-1)/9 */
    seq_t part1 = (pow10_2n1 - 1) / 9;
    part1 = part1 * 5;

    /* 3*10^n */
    seq_t part2 = pow10_n * 3;

    return part1 + part2;
}
