/*
 * A240951  Maximum number of dividing subsets of a set of n natural numbers.
 * https://oeis.org/A240951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1) for n>4, with special values a(1)=1, a(2)=2,
 *            a(3)=5, a(4)=8
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)2;
    if (n == 3)
        return (seq_t)5;
    if (n == 4)
        return (seq_t)8;
    /* n > 4 */
    return pow2(n - 1);
}
