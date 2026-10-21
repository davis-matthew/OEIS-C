/*
 * A262768  a(n) = binomial(2*n+2,n) - 2*binomial(2*n,n) + binomial(2*n-2,n).
 * https://oeis.org/A262768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n+2,n) - 2*binomial(2*n,n) + binomial(2*n-2,n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle small n directly to avoid underflow in the binomial arguments */
    if (n < 2)
        return (seq_t)0;

    /* Compute the required binomials using the utility implementation */
    idx_t two_n = 2 * n;
    seq_t t1 = binomial(two_n + 2, n);   /* binomial(2*n+2, n) */
    seq_t t2 = binomial(two_n,     n);    /* binomial(2*n,   n) */
    seq_t t3 = binomial(two_n - 2, n);    /* binomial(2*n-2, n) */

    /* Arrange the linear combination = t1 - 2*t2 + t3 */
    return t1 - (2 * t2) + t3;
}
