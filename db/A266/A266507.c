/*
 * A266507  a(n) = 6*a(n - 1) - a(n - 2) with a(0) = 2, a(1) = 8.
 * https://oeis.org/A266507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2); a(0) = 2; a(1) = 8
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;
    if (n == 1)
        return (seq_t)8;

    seq_t prev2 = (seq_t)2;          /* a(0) */
    seq_t prev1 = (seq_t)8;          /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 * (seq_t)6 - prev2;  /* a(i) = 6*a(i-1) - a(i-2) */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
