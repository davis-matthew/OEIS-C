/*
 * A297619  a(n) = 2*a(n-1) + 2*a(n-2) - 4*a(n-3), a(1) = 0, a(2) = 0, a(3) =
 *          8.
 * https://oeis.org/A297619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n + 1) - 2^(floor(n/2) + 2) (n ≥ 1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Base cases automatically handled by the formula */
    if (n == 0)
        return 0;                       /* undefined by offset, return 0 */

    seq_t term1 = pow2((idx_t)(n + 1));                          /* 2^(n+1) */
    seq_t term2 = pow2((idx_t)((n >> 1) + 2));                   /* 2^(⌊n/2⌋+2) */
    return term1 - term2;                                         /* difference */
}
