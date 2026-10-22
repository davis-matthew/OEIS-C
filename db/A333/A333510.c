/*
 * A333510  Number of self-avoiding walks in the n X 2 grid graph which start
 *          at any of the n vertices on left side of the graph and terminate
 *          at any of the n vertices on the right side.
 * https://oeis.org/A333510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (27*2^n - n^3 - 26*n - 24)/3
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t */
    seq_t sn = (seq_t)n;

    /* 2^n as a seq_t */
    seq_t pow2n = pow2(n);

    /* 9*2^n (9 = 27/3) */
    seq_t pow2n9 = pow2n * 9;

    /* n^2 and n^3 */
    seq_t n2 = sn * sn;
    seq_t n3 = n2 * sn;

    /* Compute the subtraction part: (n^3 + 26*n + 24)/3 */
    seq_t sub = n3 + sn * 26 + 24;
    seq_t sub_div = sub / 3;

    /* Final result: 9*2^n - sub/3 */
    return pow2n9 - sub_div;
}
