/*
 * A288133  Positions of 0 in A288132; complement of A288134.
 * https://oeis.org/A288133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-2) + n - 1 for n ≥ 2; a(1) = 1
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t p = pow2(n - 2);        /* 2^(n-2) */
    seq_t o = (seq_t)(n - 1);     /* n - 1   */
    return p + o;                 /* a(n)    */
}
