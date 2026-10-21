/*
 * A282005  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 413", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A282005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for even n > 0: a(n) = 3*2^(n-1); for odd n: a(n) =
 *            2^n - 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
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
    if (n & 1)          /* odd n */
        return pow2(n) - (seq_t)1;
    /* even n > 0 */
    return pow2(n - 1) * (seq_t)3;
}
