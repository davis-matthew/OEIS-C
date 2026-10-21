/*
 * A277800  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 1", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A277800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n for even n, a(n) = 2^(n-1) - 1 for odd n
 *            (equivalently a(n) = (-2 + (-2)^n + 2*(-1)^n + 3*2^n)/4)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* even n → 2^n; odd n → 2^(n-1) - 1 */
    if ((n & 1ULL) == 0)
        return pow2(n);
    return pow2(n - 1) - ((seq_t)1);
}
