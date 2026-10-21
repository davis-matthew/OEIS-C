/*
 * A273310  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 641", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 4*4^n - 4*2^n - 8 for n > 1 a(0) = 1, a(1) = 4. (Values
 *            for n=0,1 are special cases)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)4;

    /* compute 4 * 4^n  = 4 * 2^(2n)  */
    seq_t term1 = (seq_t)4 * pow2((idx_t)(2 * n));
    /* compute 4 * 2^n */
    seq_t term2 = (seq_t)4 * pow2(n);
    /* a(n) = term1 - term2 - 8 */
    return term1 - term2 - (seq_t)8;
}
