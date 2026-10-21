/*
 * A279961  Decimal representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 221", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A279961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 if n>2 and even; a(n) = 2^{n+1} - 4 if n>2 and odd;
 *            a(0)=1, a(1)=2, a(2)=2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 2) {
        switch (n) {
            case 0: return (seq_t)1;
            case 1: return (seq_t)2;
            case 2: return (seq_t)2;
        }
    }
    if ((n & 1) == 0) {
        return (seq_t)7;
    } else {
        seq_t v = pow2((idx_t)n + 1);
        v -= (seq_t)4;
        return v;
    }
}
