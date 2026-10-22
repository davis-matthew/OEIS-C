/*
 * A366542  Number of discrete uninorms defined on the finite chain
 *          L_n={0,1,...n}, U:L_n^2->L_n, whose underlying operators are
 *          smooth and idempotent, or smooth and idempotent-free.
 * https://oeis.org/A366542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + n*(n+1) - 6 (n >= 3); a(1) = 2, a(2) = 6
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"  // for pow2(idx_t)

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;
    /* for n >= 3 */
    seq_t nn = (seq_t)n;          /* convert index to seq_t */
    seq_t tri = nn * (nn + 1);    /* n*(n+1) */
    seq_t term = pow2(n);         /* 2^n as seq_t */
    return term + tri - (seq_t)6;
}
