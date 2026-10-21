/*
 * A273251  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 619", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (10*4^n - 12*2^n - 1)/3 for n>0; a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t pow4_n = pow4(n);       /* 4^n  */
    seq_t pow2_n = pow2(n);       /* 2^n  */

    seq_t numerator = (seq_t)10 * pow4_n - (seq_t)12 * pow2_n - (seq_t)1;
    return numerator / (seq_t)3;
}
