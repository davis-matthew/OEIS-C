/*
 * A374721  Number of nonisomorphic spanning trees of the triangular snake
 *          graph nC_3.
 * https://oeis.org/A374721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*3^(n-2) + 3^floor((n-2)/2) (n ≥ 1, a(1)=1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;               /* base case */
    idx_t exp1 = n - 2;                 /* exponent for 3^(n-2) */
    seq_t part1 = pow3(exp1);           /* 3^(n-2) */
    part1 = part1 * 2;                  /* 2*3^(n-2) */
    idx_t exp2 = exp1 / 2;              /* floor((n-2)/2) */
    seq_t part2 = pow3(exp2);           /* 3^floor((n-2)/2) */
    return part1 + part2;               /* sum */
}
