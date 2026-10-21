/*
 * A288763  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 501", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A288763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(1)=1; a(2)=4; a(3)=7; a(4)=24; a(5)=31; For n>5: if n
 *            even: a(n)=3*2^(n-1) if n odd: a(n)=2^(n-1)-1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)1;
        case 2: return (seq_t)4;
        case 3: return (seq_t)7;
        case 4: return (seq_t)24;
        case 5: return (seq_t)31;
        default:
            if ((n & 1) == 0) {
                return ((seq_t)3) * pow2(n - 1);
            } else {
                return pow2(n - 1) - (seq_t)1;
            }
    }
}
