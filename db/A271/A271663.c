/*
 * A271663  Convolution of nonzero squares (A000290) with nonzero pentagonal
 *          numbers (A000326).
 * https://oeis.org/A271663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+5,5) + 3*binomial(n+4,5) + 2*binomial(n+3,5)
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 151, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Using the supplied binomial helper (fast for small k). */

#include <stddef.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute the three binomial terms.  k = 5 is small, so binomial() is fast. */
    seq_t t1 = binomial(n + 5, 5);   /* binom(n+5,5)   */
    seq_t t2 = binomial(n + 4, 5);   /* binom(n+4,5)   */
    seq_t t3 = binomial(n + 3, 5);   /* binom(n+3,5)   */

    seq_t res = t1;                  /* start with first term */

    /* Add 3 * t2 by three successive additions (avoids overflow for 64-bit). */
    res += t2;
    res += t2;
    res += t2;

    /* Add 2 * t3 by two successive additions. */
    res += t3;
    res += t3;

    return res;
}
