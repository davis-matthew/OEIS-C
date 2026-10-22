/*
 * A378924  Number of minimal edge cuts in the n-Plummer-Toft graph.
 * https://oeis.org/A378924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*2^n + (n+2)*(n+7)/2
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t pow2  = pow_seq((seq_t)2, (int)n);   /* 2^n */
    seq_t term1 = (seq_t)5 * pow2;             /* 5*2^n */
    seq_t nn    = (seq_t)n;
    seq_t term2 = (nn + (seq_t)2) * (nn + (seq_t)7); /* (n+2)(n+7) */
    term2 /= (seq_t)2;                         /* divide by 2 */
    return term1 + term2;
}
