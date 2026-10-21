/*
 * A286191  a(n) = (2^n-1)^2 + 2*n.
 * https://oeis.org/A286191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n-1)^2 + 2*n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    seq_t p = pow2(n);           /* 2^n   */
    seq_t p1 = p - 1;            /* 2^n-1 */
    seq_t sq = p1 * p1;          /* (2^n-1)^2 */
    seq_t add = (seq_t)n * 2;    /* 2*n   */
    return sq + add;
}
