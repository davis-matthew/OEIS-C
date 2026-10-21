/*
 * A278547  Number of n X 1 0..3 arrays with rows and columns in lexicographic
 *          nondecreasing order but with exactly two mistakes.
 * https://oeis.org/A278547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n+11,n) - (n+1)*binomial(n+7,n) +
 *            binomial(n+3,n)*binomial(n+1,2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 23, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t t1 = binom(n + 11, n);   /* binomial(n+11,n) = binomial(n+11,11) */
    seq_t t2 = binom(n + 7,  n);   /* binomial(n+7,n)   = binomial(n+7,7)  */
    seq_t t3 = binom(n + 3,  n);   /* binomial(n+3,n)   = binomial(n+3,3)  */
    seq_t twos = binom(n + 1, 2);   /* binomial(n+1,2)                      */

    return t1 - (seq_t)(n + 1) * t2 + t3 * twos;
}
