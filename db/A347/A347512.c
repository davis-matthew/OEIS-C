/*
 * A347512  Number of minimal dominating sets in the n-book graph.
 * https://oeis.org/A347512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=6, a(n)=2^n+3 for n>1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)6;
    return pow2(n) + (seq_t)3;
}
