/*
 * A244495  Number of 3 X 3 matrices of nonnegative integer entries with all
 *          row and column sums <= n.
 * https://oeis.org/A244495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = \sum_{i = 0}^{\min(6,n)} C(n-i+9,9) * c[i] where c = [1,
 *            24, 156, 280, 156, 24, 1] (numerator of the generating function
 *            (1+24x+156x^2+280x^3+156x^4+24x^5+x^6)/(1-x)^10)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 21, u64 n <= 277,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Numerical coefficients from the generating function numerator. */
    static const uint16_t coeff[7] = {1, 24, 156, 280, 156, 24, 1};

    seq_t sum = 0;

    /* The sum is over i = 0..6, but we stop earlier if n < 6. */
    idx_t max_i = (n < 6 ? n : 6);

    for (idx_t i = 0; i <= max_i; ++i)
    {
        /* n-i+9 is always >= 9 because i <= n. */
        idx_t m = n - i + 9;

        /* Compute C(m,9) multiplicatively in seq_t arithmetic. */
        seq_t binom = 1;
        for (int k = 1; k <= 9; ++k)
        {
            binom = binom * (seq_t)(m - k + 1) / (seq_t)k;
        }

        /* Accumulate the weighted contribution. */
        sum += binom * (seq_t)coeff[i];
    }

    return sum;
}
