/*
 * A386876  a(n) = (1/2) * (3*n)! / n!^3 for n > 0, a(0) = 1.
 * https://oeis.org/A386876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(3*n, n) * binomial(2*n, n) / 2 , a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    seq_t c1 = binomial(2 * n, n);   /* central binomial coefficient, even for n>0 */
    c1 /= 2;                          /* divide first to keep intermediate sizes small */
    seq_t c2 = binomial(3 * n, n);    /* choose n from 3n */
    return c1 * c2;
}
