/*
 * A270088  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 73", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 4*4^n - 4*2^n + 1 for n > 2 Initial values: a(0) = 1,
 *            a(1) = 4, a(2) = 40 Recurrence for n>4 (not used directly): a(n)
 *            = 7*a(n-1) - 14*a(n-2) + 8*a(n-3)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Handle small indices explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)40;

    /* Compute 2^(2n) = 4^n and 2^n */
    seq_t pow4n = pow2(2 * n);   /* 2^(2n) */
    pow4n *= (seq_t)4;           /* 4 * 4^n */

    seq_t pow2n = pow2(n);
    pow2n *= (seq_t)4;           /* 4 * 2^n */

    /* a(n) = 4*4^n - 4*2^n + 1 */
    return pow4n - pow2n + (seq_t)1;
}
