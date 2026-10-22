/*
 * A347478  Number of total dominating sets in the n-alkane graph.
 * https://oeis.org/A347478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15 for n=1; a(n) = 4^(n+1) for n>1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)15;
    /* n > 1: compute 4^(n+1) */
    return pow4(n + 1);
}
