/*
 * A273828  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 950", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2*4^n - 2*2^n - 3 for n > 2, a(0) = 1, a(1) = 5, a(2) =
 *            25
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)25;

    /* Compute 2*4^n = 2 * 2^(2n) */
    seq_t p4 = pow2(2 * n);
    p4 += p4;                 /* multiply by 2 */

    /* Compute 2*2^n = 2 * 2^n */
    seq_t p2 = pow2(n);
    p2 += p2;                 /* multiply by 2 */

    return p4 - p2 - (seq_t)3;
}
