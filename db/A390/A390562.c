/*
 * A390562  Upper (1/5,1/2) midsequence of (2^n) and (5*n); see Comments.
 * https://oeis.org/A390562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil( 2^n/5 + (5*n)/2 ) a(0) = 1, a(1) = 3, a(2) = 6,
 *            a(3) = 10, a(4) = 14
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 30, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* compute 2^n */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow2 = pow2 * (seq_t)2;

    /* numerator = 2*2^n + 25*n + 9  (since ceil((2*2^n + 25*n)/10) = (2*2^n + 25*n + 9)/10) */
    seq_t numerator = pow2 * (seq_t)2;
    numerator += ((seq_t)25) * ((seq_t)n);
    numerator += (seq_t)9;

    return numerator / (seq_t)10;
}
