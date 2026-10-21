/*
 * A253155  Number of (n+1) X (4+1) 0..1 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally and
 *          vertically.
 * https://oeis.org/A253155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*2^(n-1) + 92 for n >= 4 a(1) = 109 a(2) = 120 a(3) =
 *            129
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)109;
    if (n == 2) return (seq_t)120;
    if (n == 3) return (seq_t)129;

    /* For n >= 4, use the explicit formula */
    seq_t two = pow2(n - 1);          /* 2^(n-1) */
    return two * (seq_t)9 + (seq_t)92;
}
