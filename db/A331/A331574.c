/*
 * A331574  a(n) is the number of subsets of {1..n} that contain 3 even and 3
 *          odd numbers.
 * https://oeis.org/A331574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(floor(n/2),3) * binomial(ceil(n/2),3)
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* If n < 6 there are not enough elements to select 3 evens and 3 odds. */
    if (n < 6)
        return 0;

    /* Number of even elements in {1..n} is floor(n/2). */
    idx_t evens = n / 2;
    /* Number of odd elements is n - evens (i.e. ceil(n/2)). */
    idx_t odds  = n - evens;

    /* Compute the binomial coefficients.  binomial returns seq_t. */
    seq_t ce = binomial(evens, 3);   /* choose 3 even numbers */
    seq_t co = binomial(odds, 3);    /* choose 3 odd numbers  */

    /* The desired count is the product of the two choices. */
    return ce * co;
}
