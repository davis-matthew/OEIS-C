/*
 * A269907  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 1", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A269907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 4*(4^n-2^n-1) for n>0, a(0)=1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t pow4 = pow_int((seq_t)4, n);
    seq_t pow2 = pow_int((seq_t)2, n);
    seq_t tmp  = pow4 - pow2 - (seq_t)1;
    return (seq_t)4 * tmp;
}
