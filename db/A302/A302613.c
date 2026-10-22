/*
 * A302613  Total domination number of the n-folded cube graph.
 * https://oeis.org/A302613
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2^(n-4) for n ≥ 8; special cases: a(2) = a(3) = a(4) = 2,
 *            a(5) = 4, a(6) = 8, a(7) = 12
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 4)
        return (seq_t)2;
    if (n == 5)
        return (seq_t)4;
    if (n == 6)
        return (seq_t)8;
    if (n == 7)
        return (seq_t)12;
    /* n ≥ 8 */
    return pow2(n - 4);
}
