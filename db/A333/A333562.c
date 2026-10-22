/*
 * A333562  a(n) = Sum_{j = 0..3*n} binomial(n+j-1,j)*2^j.
 * https://oeis.org/A333562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j = 0 .. 3*n} binomial(n+j-1, j)*2^j
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the utility functions `binomial` and `pow2`. */

#include <stdbool.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t a, idx_t b)  */
#include "../utils/pow2.h"       /* provides seq_t pow2(idx_t exp)          */

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition of the empty sum. */
    if (n == 0)
        return (seq_t)1;

    idx_t limit = 3 * n;          /* upper bound for j   */
    seq_t sum   = 0;              /* accumulator        */

    for (idx_t j = 0; j <= limit; ++j)
    {
        /* binomial coefficient C(n+j-1, j) */
        seq_t comb = binomial(n + j - 1, j);
        /* 2^j                                           */
        seq_t pow  = pow2(j);
        /* add term comb * Pow to the sum                */
        sum += comb * pow;
    }
    return sum;
}
