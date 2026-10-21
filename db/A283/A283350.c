/*
 * A283350  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 617", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A283350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - 2 for n>3 even; 2^n - 8 for n>3 odd
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=1, a(1)=0, a(2)=6, a(3)=8 */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    switch (n) {
        case 0: return 1;
        case 1: return 0;
        case 2: return 6;
        case 3: return 8;
        default:
            seq_t val = pow2(n);
            if ((n & 1ULL) == 0)
                return val - (seq_t)2;
            else
                return val - (seq_t)8;
    }
}
