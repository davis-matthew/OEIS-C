/*
 * A270007  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 5", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2^(2n+2) - 2^(n+2) + 1 for all n ≠ 1; a(1) = 8. (This
 *            single exception occurs in the sequence.) The formula is derived
 *            from the observed pattern: a(n) = 4^{n+1} - 2^{n+2} + 1, which
 *            holds for n = 0 and n ≥ 2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)8;          /* special case for n = 1 */
    /* compute 2^(2n+2) and 2^(n+2) */
    seq_t high = pow2((n << 1) + 2);   /* 2^(2n+2) */
    seq_t low  = pow2(n + 2);          /* 2^(n+2)   */
    return high - low + (seq_t)1;      /* high - low + 1 */
}
