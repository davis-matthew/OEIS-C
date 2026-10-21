/*
 * A287470  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 276", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A287470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) - 2^(n/2) for n even; a(n) = 0 for n odd
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (OEIS A287470) */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Odd indices yield zero. */
    if ((n & 1U) != 0ULL)
        return 0;

    /* Even indices: compute 2^(n+1) - 2^(n/2).  */
    seq_t high = pow2(n + 1);
    seq_t low  = pow2(n / 2);
    return high - low;
}
