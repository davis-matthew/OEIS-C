/*
 * A282577  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 553", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A282577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0; for n>1: a(n)=0 if n is odd, a(n)=(2^n-1)/3 if n
 *            is even. (Sequence A282577)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 33, u64 n <= 65, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 0;
    if (n & 1)  return 0;          /* odd n > 1 */
    return (pow2(n) - 1) / 3;      /* even n > 1 */
}
