/*
 * A348621  The number of additions required to compute the permanent of
 *          general n X n matrices using Ryser's formula without Gray code
 *          ordering.
 * https://oeis.org/A348621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 - 2*n + 2)*2^(n-1) + n - 2
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */
    seq_t nn = (seq_t)n;
    seq_t coeff = nn * nn - (seq_t)2 * nn + (seq_t)2;
    seq_t power = pow2(n - 1);     /* 2^(n-1) */
    seq_t prod  = coeff * power;
    seq_t result = prod + nn - (seq_t)2;
    return result;
}
