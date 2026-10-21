/*
 * A277798  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 1", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A277798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n if n is even * a(1) = 0 if n = 1 * a(n) =
 *            (10^(n-1)-1)/9 if n is odd and n > 1 * This follows the pattern
 *            of the sequence’s binary representation * for Rule 1 on the
 *            5‑cell von Neumann neighbourhood
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* even indices */
    if ((n & 1u) == 0u)
        return pow_uint(10, n);

    /* odd indices */
    if (n == 1u)
        return 0;

    seq_t p = pow_uint(10, n - 1u);
    return (p - 1) / 9;
}
