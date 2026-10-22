/*
 * A352847  Number of copies of the star graph S(2,1,1) contained within the
 *          n-dimensional hypercube graph.
 * https://oeis.org/A352847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*binomial(n,2)*(n-1)*(n-2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    seq_t res = pow2(n-1);           /* 2^(n-1) */
    res *= (seq_t)n;                 /* * n */
    res *= (seq_t)(n-1);             /* * (n-1) */
    res *= (seq_t)(n-1);             /* * (n-1) again */
    res *= (seq_t)(n-2);             /* * (n-2) */
    return res;
}
