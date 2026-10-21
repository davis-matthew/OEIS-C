/*
 * A273389  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 662", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 5*4^(n-1) - 2^(n-1) - 51 for n>5; a(0)=1, a(1)=5,
 *            a(2)=21, a(3)=81, a(4)=297, a(5)=1201
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    switch (n) {
        case 0: return (seq_t)1;
        case 1: return (seq_t)5;
        case 2: return (seq_t)21;
        case 3: return (seq_t)81;
        case 4: return (seq_t)297;
        case 5: return (seq_t)1201;
        default:
            seq_t pow4 = pow_int((seq_t)4, n - 1);
            seq_t pow2 = pow_int((seq_t)2, n - 1);
            return (seq_t)5 * pow4 - pow2 - (seq_t)51;
    }
}
