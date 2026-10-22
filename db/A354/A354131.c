/*
 * A354131  Number of tilings of a 2 X n rectangle using 2 X 2 and 1 X 1
 *          tiles, right trominoes and dominoes.
 * https://oeis.org/A354131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 6*a(n-2) with initial values a(0) = 1, a(1) =
 *            2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t, required by some compilers */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    seq_t prev2 = (seq_t)1;
    seq_t prev1 = (seq_t)2;
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 * (seq_t)3 + prev2 * (seq_t)6;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
