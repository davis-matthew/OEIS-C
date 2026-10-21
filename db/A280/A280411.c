/*
 * A280411  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 261", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A280411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n if n even; (10^n - 1)/9 if n odd
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t p = pow10(n);
    if ((n & 1) == 0)
        return p;            /* even n: 10^n */
    else
        return (p - 1) / 9;  /* odd n: (10^n - 1)/9 */
}
