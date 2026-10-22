/*
 * A362438  a(n) = n^2 + 2^(n-1).
 * https://oeis.org/A362438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 + 2^(n-1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Sequences are defined for n >= 1.  For n < 1 the function returns 0. */
    if (n < 1U)
        return 0;

    seq_t sq = (seq_t)n * (seq_t)n;      /* n^2 in the desired type   */
    seq_t p  = pow2(n - 1);              /* 2^(n-1)                    */
    return sq + p;                       /* n^2 + 2^(n-1)              */
}
