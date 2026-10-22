/*
 * A347477  Number of total dominating sets in the complement graph of the
 *          n-cycle.
 * https://oeis.org/A347477
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - 4*n - 1 for n>4; a(3)=0; a(4)=1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Values for the offset < 5 are fixed */
    if (n == 3) return (seq_t)0;
    if (n == 4) return (seq_t)1;
    if (n <= 2) return (seq_t)0;  /* arbitrary for unsupported n */

    /* 2^n is computed by the helper pow2 to handle arbitrary precision */
    seq_t term = pow2(n) - (seq_t)4 * (seq_t)n - (seq_t)1;
    return term;
}
