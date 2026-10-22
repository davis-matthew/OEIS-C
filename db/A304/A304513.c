/*
 * A304513  a(n) = 57*2^(n-1) - 38 (n >= 1).
 * https://oeis.org/A304513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 57*2^(n-1) - 38 (n>=1)
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset starts at 1 */
    seq_t p = pow2(n - 1); /* 2^(n-1) */
    seq_t r = (seq_t)57 * p;
    r -= (seq_t)38;
    return r;
}
