/*
 * A286772  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 221", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A286772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=0; for n≥3 odd a(n)=2^(n+1)−2; even a(n)=1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)0;

    if ((n & 1u) == 0) /* even index */
        return (seq_t)1;
    else /* odd index */
    {
        seq_t v = pow2(n + 1); /* 2^(n+1) */
        return v - (seq_t)2;
    }
}
