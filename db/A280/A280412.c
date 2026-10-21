/*
 * A280412  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 261", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A280412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for even n; a(n) = 2^(n+1)-2 for odd n
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* if n is even, the answer is trivially 1 */
    if ((n & 1) == 0)
        return (seq_t)1;

    /* n is odd: compute 2^(n+1) - 2 */
    seq_t val = pow2(n + 1);   /* 2 raised to the power (n+1)  */
    val -= (seq_t)2;           /* subtract 2 */
    return val;
}
