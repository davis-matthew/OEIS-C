/*
 * A287197  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 259", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A287197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is even * a(1) = 3 * a(n) = 2^(n+1) – 4 if n is
 *            odd and n > 1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0)           /* even n */
        return (seq_t)1;

    if (n == 1)                 /* special case */
        return (seq_t)3;

    /* odd n >= 3: 2^(n+1) – 4 */
    seq_t p = pow2(n + 1);
    return (p - (seq_t)4);
}
