/*
 * A277936  Decimal representation of the x-axis, from the left edge to the
 *          origin, or from the origin to the right edge, of the n-th stage of
 *          growth of the two-dimensional cellular automaton defined by "Rule
 *          7", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A277936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(2k)=0 (k>0), a(2k+1)=2^(2k+2)-1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if ((n & 1) == 0)   /* even index n>0 */
        return (seq_t)0;
    /* odd index: a(n) = 2^(n+1)-1 */
    seq_t val = pow2(n + 1);
    return val - (seq_t)1;
}
