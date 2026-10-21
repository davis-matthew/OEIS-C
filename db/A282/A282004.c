/*
 * A282004  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 413", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A282004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = 3 for n > 0 even; a(n) = 2^(n+1) - 2 for n odd
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;
    if ((n & (idx_t)1) == (idx_t)0)   /* even n > 0 */
        return (seq_t)3;
    /* odd n */
    seq_t val = pow2(n + (idx_t)1);
    return val - (seq_t)2;
}
