/*
 * A340257  a(n) = 2^n * (1+n*(n+1)/2).
 * https://oeis.org/A340257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * (1 + n*(n+1)/2)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 53, u128 n <= 115,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Compute A340257: a(n) = 2^n * (1 + n*(n+1)/2)  */
seq_t a(idx_t n)
{
    seq_t power = pow2(n);                                 /* 2^n      */
    seq_t tri   = (seq_t)n * ((seq_t)n + 1) / 2;           /* n*(n+1)/2 */
    seq_t sum   = tri + (seq_t)1;                          /* 1 + tri   */
    return power * sum;                                    /* final term*/
}
