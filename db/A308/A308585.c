/*
 * A308585  a(n) = 2^(n + 3) - 10*n - 6.
 * https://oeis.org/A308585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+3) - 10*n - 6
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. For n = 0 we return 0 as
       the behaviour for indices below the offset is irrelevant. */
    if (n == 0)
        return (seq_t)0;

    seq_t val = pow2(n + 3);          /* 2^(n+3) */
    val -= (seq_t)10 * n;             /* subtract 10*n   */
    val -= (seq_t)6;                  /* subtract 6       */
    return val;
}
