/*
 * A326998  a(n) = 1 + binomial(3*n-1, n) + binomial(3*n-1,
 *          n-1)*(binomial(2*n-1, n) + 1).
 * https://oeis.org/A326998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + binomial(3*n-1, n) + binomial(3*n-1,
 *            n-1)*(binomial(2*n-1, n) + 1) Offset: 0 (a(0) = 4)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Special case for the offset because 3*0-1 is negative */
    if (n == 0)
        return (seq_t)4;

    idx_t three_n_minus_one = 3 * n - 1;
    idx_t two_n_minus_one   = 2 * n - 1;

    seq_t c1 = binomial(three_n_minus_one, n);          /* C(3n-1, n)   */
    seq_t c2 = binomial(three_n_minus_one, n - 1);      /* C(3n-1, n-1) */
    seq_t c3 = binomial(two_n_minus_one, n);            /* C(2n-1, n)   */

    seq_t prod = c2 * (c3 + 1);          /* binom(3n-1, n-1) * (binom(2n-1, n) + 1) */

    return 1 + c1 + prod;
}
