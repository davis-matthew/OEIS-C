/*
 * A332044  a(n) is the length of the shortest circuit that visits every edge
 *          of an undirected n X n grid graph.
 * https://oeis.org/A332044
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n^2 - 4 + 2*(n mod 2)
 * Range:     u8 n <= 11, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;          /* behaviour below the offset does not matter */

    seq_t nn = (seq_t)n;   /* cast once to seq_t */
    seq_t res = nn * nn;   /* n^2 */
    res *= 2;              /* 2*n^2 */
    res -= 4;              /* 2*n^2 - 4 */
    res += (seq_t)(2 * (n & 1)); /* + 2*(n mod 2) */
    return res;
}
