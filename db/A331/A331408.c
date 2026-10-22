/*
 * A331408  Number of subsets of {1..n} that contain exactly three odd
 *          numbers.
 * https://oeis.org/A331408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C( ceil(n/2) , 3 ) * 2^floor(n/2) for n ≥ 1 (where C is
 *            the binomial coefficient and 2^x is power of two)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 43, u64 n <= 99, u128 n <= 221,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For n < 5 there are fewer than 3 odd numbers, so the answer is 0. */
    if (n < 5)
        return 0;

    /* Number of odd integers in {1..n} is ceil(n/2). */
    idx_t odds   = (n + 1) / 2;   /* ceil(n/2) */
    idx_t evens  = n / 2;         /* floor(n/2) */

    /* Compute C(odds, 3) and 2^evens, then multiply. */
    seq_t comb = binomial(odds, 3);
    seq_t pow  = pow2(evens);

    return comb * pow;
}
