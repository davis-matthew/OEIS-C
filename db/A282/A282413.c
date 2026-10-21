/*
 * A282413  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 467", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A282413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 1; For n > 1: if n is odd, a(n) = 0; if n is
 *            even, a(n) = 2^(n+1) - 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper pow2 from "../utils/pow2.h". */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 1u)            /* a(0) and a(1) */
        return (seq_t)1;

    if (n & 1u)             /* odd n > 1 */
        return (seq_t)0;

    /* even n > 1 */
    seq_t val = pow2(n + 1);
    val -= (seq_t)1;
    return val;
}
