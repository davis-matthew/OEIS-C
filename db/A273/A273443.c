/*
 * A273443  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 721", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n*(n+1) for n > 3; a(0)=1, a(1)=4, a(2)=21, a(3)=41
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    switch (n) {
        case 0: return 1;
        case 1: return 4;
        case 2: return 21;
        case 3: return 41;
        default:
        {
            seq_t nn = (seq_t)n;
            return (seq_t)4 * nn * (nn + 1);
        }
    }
}
