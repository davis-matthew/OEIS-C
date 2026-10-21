/*
 * A280369  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 259", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A280369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1; for n>1 even: a(n)=3; for n>1 odd:
 *            a(n)=2^(n+1)-8
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return 1;
    if ((n & 1) == 0)  /* even n > 1 */
        return 3;
    /* odd n > 1 */
    seq_t v = pow2(n + 1);  /* 2^(n+1) */
    v -= 8;
    return v;
}
