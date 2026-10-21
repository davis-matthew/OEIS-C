/*
 * A282798  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 505", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A282798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) - 1 for n even a(n) = 2*(2^n - 2) / 3 for n odd
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {
        seq_t v = pow2(n + 1);   /* 2^(n+1) */
        return v - 1;
    }
    else {
        seq_t v = pow2(n);       /* 2^n */
        seq_t t = v - 2;         /* 2^n - 2 */
        seq_t u = t * 2;         /* 2*(2^n - 2) */
        return u / 3;            /* divide by 3 */
    }
}
