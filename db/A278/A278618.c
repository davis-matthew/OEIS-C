/*
 * A278618  a(n) = Sum_{j=0..n/2} binomial(n-j-1,n-2*j)*binomial(2*n+1,j).
 * https://oeis.org/A278618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{j=0}^{⌊n/2⌋} C(n−j−1, n−2j) * C(2n+1, j)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the provided binom function to evaluate binomial
   coefficients.  It handles the case j = 0 implicitly by skipping it,
   because C(n−1, n) = 0 for all n ≥ 1.  For n = 0 the result is 1.  */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t sum = 0;
    idx_t half = n / 2;               /* floor(n/2) */

    for (idx_t j = 1; j <= half; ++j)
    {
        /* a1 = n-j-1, b1 = n-2j  (C(n-j-1, n-2j)) */
        idx_t a1 = n - j - 1;
        idx_t b1 = n - 2 * j;

        /* if b1 > a1 the binomial coefficient is 0; skip */
        if (b1 > a1)
            continue;

        seq_t term1 = binom(a1, b1);
        seq_t term2 = binom(2 * n + 1, j);
        sum += term1 * term2;
    }

    return sum;
}
