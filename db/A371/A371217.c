/*
 * A371217  The maximum deck size to perform Colm Mulcahy's n-card trick.
 * https://oeis.org/A371217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (n-1)*(1 + 2*Sum_{i=1..n-1} (i-1)!*binomial(n-1, i))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute factorials (i-1)! and binomial coefficients binom(n-1,i)
       iteratively to avoid large intermediate growth */
    seq_t fact = 1;                /* (i-1)! for current i */
    seq_t binom = 1;               /* binomial(n-1, i-1), starting at i=0 */
    seq_t sum = 0;

    /* loop over i = 1 .. n-1 */
    for (idx_t i = 1; i < n; ++i) {
        /* compute binomial(n-1,i) from previous value:
           binom = binom * (n - i) / i  */
        binom = (binom * (seq_t)(n - i)) / (seq_t)i;

        /* term = (i-1)! * binomial(n-1, i) */
        seq_t term = fact * binom;
        sum += term;

        /* update factorial for next i: (i)! = fact * i */
        fact *= i;
    }

    seq_t nm1 = (seq_t)(n - 1);
    seq_t result = 1 + nm1 * (1 + 2 * sum);
    return result;
}
