/*
 * A279118  Binary representation of the x-axis, from the left edge to the
 *          origin, (and also from the origin to the right edge) of the n-th
 *          stage of growth of the two-dimensional cellular automaton defined
 *          by "Rule 209", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A279118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0, if n is odd; otherwise a(n) = (10^(n+1) - 1) / 9
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The values are decimal numbers consisting of (n+1) ones.                */
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n & (idx_t)1) {
        return (seq_t)0;
    }
    idx_t e = n + (idx_t)1;          /* exponent: 2k+1 when n = 2k          */
    seq_t p = pow10(e);              /* compute 10^e                       */
    return (p - (seq_t)1) / (seq_t)9;/* divide by 9 to get the repeated 1's */
}
