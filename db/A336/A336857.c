/*
 * A336857  The digit sum of n*(n^2 + 1)/2.
 * https://oeis.org/A336857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digit_sum(n*(n*n + 1)/2)
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* convert n to the appropriate sequence type first */
    seq_t nn = (seq_t)n;
    seq_t val = nn * (nn * nn + 1) / 2;
    return digit_sum(val);
}
