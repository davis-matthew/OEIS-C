/*
 * A364145  a(n) is the sum of the first 2*n nonzero n-bonacci numbers.
 * https://oeis.org/A364145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*4^n - (n-1)*2^n)/4 for n>=1; a(0)=0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 0 */
        return (seq_t)0;

    seq_t four_pow = pow4(n);            /* 4^n */
    seq_t two_pow  = pow2(n);            /* 2^n */

    seq_t numer = (seq_t)2 * four_pow   /* 2*4^n */
                 - (seq_t)(n - 1) * two_pow;  /* -(n-1)*2^n */

    return numer / (seq_t)4;            /* divide by 4 */
}
