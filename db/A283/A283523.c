/*
 * A283523  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 913", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A283523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0, a(2)=5, a(3)=14, a(n)=2^(n+1)-1 for n>=4
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)0;
        case 2: return (seq_t)5;
        case 3: return (seq_t)14;
        default: {
            seq_t val = pow2((idx_t)(n + 1));
            return val - (seq_t)1;
        }
    }
}
