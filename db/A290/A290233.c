/*
 * A290233  Binary representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 773", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^e - 1) / 9 where e = n + 1 if n is even, e = (n + 1)
 *            / 2 if n is odd
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Determine the exponent e according to the parity of n */
    idx_t e = ((n & 1) == 0) ? (n + 1) : ((n + 1) >> 1);

    /* 10^e as a seq_t (utility provided by the build) */
    seq_t ten_e = pow10(e);

    /* Return (10^e - 1) / 9 */
    return (ten_e - 1) / 9;
}
