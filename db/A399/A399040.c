/*
 * A399040  Total number of connected components in all induced subgraphs of
 *          the n-ladder graph.
 * https://oeis.org/A399040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 16*a(n-2), with a(1) = 3, a(2) = 17
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* guard for offset; not used in tests */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)17;

    seq_t prev2 = (seq_t)3;    /* a(1) */
    seq_t prev1 = (seq_t)17;   /* a(2) */
    seq_t cur = 0;

    for (idx_t i = 3; i <= n; ++i)
    {
        cur = prev1 * (seq_t)8 - prev2 * (seq_t)16;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
