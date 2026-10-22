/*
 * A399340  a(n) = Product_{j=0..n-1} binomial(n + j, n).
 * https://oeis.org/A399340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{j=0..n-1} binomial(n + j, n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0)
        return (seq_t)1;

    seq_t product = (seq_t)1;
    seq_t binom   = (seq_t)1;   /* binomial(n + 0, n) = 1 */

    /* Iterate over j = 1 .. n-1 computing binomial(n+j, n) from the
       previous one using binomial(n+j, n) = binomial(n+(j-1), n) *
       (n+j) / j.  All operations are integer and exact. */
    for (idx_t j = 1; j < n; ++j) {
        binom = binom * (seq_t)(n + j) / (seq_t)j;
        product = product * binom;
    }

    return product;
}
