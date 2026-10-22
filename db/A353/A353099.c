/*
 * A353099  a(1) = 7; for n>1, a(n) = 8 * a(n-1) + 8 - n.
 * https://oeis.org/A353099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8 * a(n-1) + 8 - n, a(1) = 7 (OEIS A353099)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the zero offset simply; the sequence is defined for n>=1. */
    if (n == 0)
        return (seq_t)0;

    seq_t x = (seq_t)7;          /* a(1) */
    if (n == 1)
        return x;

    /* Iteratively apply a(n) = 8*a(n-1) + 8 - n. */
    for (idx_t i = 2; i <= n; ++i)
        x = (seq_t)8 * x + (seq_t)8 - (seq_t)i;

    return x;
}
