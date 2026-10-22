/*
 * A397978  Lower (1/2,1) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A397978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((5*n^2 + n)/4)
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert index to seq_t and perform arithmetic using
       the sequence type to avoid overflow in intermediate
       calculations. */
    seq_t ns = (seq_t)n;
    seq_t val = ( ( (seq_t)5 * ns * ns ) + ns ) / (seq_t)4;
    return val;
}
