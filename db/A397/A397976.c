/*
 * A397976  Lower (1,3/2) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A397976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n*(n+1)/2 + (3*n*n)/2 ) = n*(4*n+1)/2
 * Range:     u8 n <= 11, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute n*(4*n+1)/2 using only integer arithmetic */
    return nn * ((seq_t)4 * nn + (seq_t)1) / (seq_t)2;
}
