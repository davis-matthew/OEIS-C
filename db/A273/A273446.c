/*
 * A273446  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 721", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0)=1, a(1)=4, a(2)=41, a(n)=4*4^n - 4*2^n for n>2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    } else if (n == 1) {
        return (seq_t)4;
    } else if (n == 2) {
        return (seq_t)41;
    } else {
        seq_t pow4 = pow_seq((seq_t)4, (int)n);
        seq_t pow2 = pow_seq((seq_t)2, (int)n);
        return (seq_t)4 * pow4 - (seq_t)4 * pow2;
    }
}
