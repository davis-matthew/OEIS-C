/*
 * A331211  Number of green nodes in n-th power graph W exponentiation of a
 *          cycle graph with 7 blue nodes and one green node.
 * https://oeis.org/A331211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 3*a(n-2); a(0)=1, a(1)=15
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 15;

    seq_t prev2 = 1;   /* a(0) */
    seq_t prev1 = 15;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 * 8 - prev2 * 3;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
