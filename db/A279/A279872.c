/*
 * A279872  Decimal representation of the x-axis, from the left edge to the
 *          origin, (and also from the origin to the right edge) of the n-th
 *          stage of growth of the two-dimensional cellular automaton defined
 *          by "Rule 209", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A279872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n is odd; a(n) = 2^(n+1) - 1 if n is even
 *            (recurrence: a(n)=5*a(n-2)-4*a(n-4) for n>3, with
 *            a(0)=1,a(1)=0,a(2)=7,a(3)=0)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* if n is odd, the term is zero */
    if (n & 1ULL)
        return 0;

    /* for even n: a(n) = 2^(n+1) - 1 */
    return pow2(n + 1) - 1;
}
