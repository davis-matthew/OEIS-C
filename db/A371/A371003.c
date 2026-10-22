/*
 * A371003  a(n) = binomial(2*n-1,n) - binomial(n,2)*(binomial(n-1,2) + 2) -
 *          1.
 * https://oeis.org/A371003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n-1,n) - binomial(n,2)*(binomial(n-1,2) + 2) -
 *            1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For n = 0 (outside the offset)
       the value is irrelevant; we simply return 0. */
    if (n == 0)
        return 0;

    /* Term 1: C(2n-1, n) */
    seq_t term1 = binomial(2 * n - 1, n);

    /* Term 2: C(n, 2) * ( C(n-1, 2) + 2 ) */
    seq_t t2 = binomial(n, 2);
    seq_t t3 = binomial(n - 1, 2);
    t3 += 2;
    t2 = t2 * t3;

    /* Final result */
    seq_t result = term1 - t2 - 1;
    return result;
}
