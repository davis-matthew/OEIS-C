/*
 * A286508  Binary representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 190", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A286508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^{floor((n+1)/2)}
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* compute e = floor((n+1)/2) safely (no overflow) */
    idx_t e = (n >> 1) + (n & 1);
    return pow10(e);
}
