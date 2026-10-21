/*
 * A285646  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 65", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A285646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^(n-1) for n>4 and even, a(n) = 2^(n-1)-1 for n>4 and
 *            odd; initial values: a(0)=1, a(1)=1, a(2)=4, a(3)=3, a(4)=16
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
        case 0: return 1;
        case 1: return 1;
        case 2: return 4;
        case 3: return 3;
        case 4: return 16;
        default:
            if ((n & 1) == 0) {
                seq_t v = pow2(n - 1);
                return v * 3;
            } else {
                seq_t v = pow2(n - 1);
                return v - 1;
            }
    }
}
