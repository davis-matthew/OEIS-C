/*
 * A286778  Sum of the common path length over all 2-tuples of nodes in a
 *          complete binary tree of height n.
 * https://oeis.org/A286778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*2^(2*n) - (4*n+2)*2^n - 2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t pow2n   = pow2(n);          /* 2^n */
    seq_t pow2_2n = pow2(2*n);        /* 2^(2n) */
    seq_t term1   = 4 * pow2_2n;      /* 4*2^(2n) */
    seq_t mult    = (seq_t)((4*n) + 2); /* (4*n+2) as seq_t */
    seq_t term2   = pow2n * mult;     /* (4*n+2)*2^n */
    return term1 - term2 - ((seq_t)2);
}
