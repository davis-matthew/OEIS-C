/*
 * A370839  Expansion of (1/x) * Series_Reversion( x * (1/(1-x^2) - x) ).
 * https://oeis.org/A370839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..floor(n/2)} (-1)^k *
 *            binomial(2*n-2*k+1, k) * binomial(2*n-2*k, n-2*k)
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 14, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Special case n == 0 handled by the loop below */
    seq_t sum = 0;
    idx_t maxk = n / 2;

    for (idx_t k = 0; k <= maxk; ++k) {
        /* Arguments for binomial are within idx_t range for the expected n. */
        idx_t m1 = 2 * n - 2 * k + 1;
        idx_t r1 = k;
        idx_t m2 = 2 * n - 2 * k;
        idx_t r2 = n - 2 * k;

        seq_t term1 = binomial(m1, r1);
        seq_t term2 = binomial(m2, r2);

        term1 *= term2;

        if (k & 1)
            sum -= term1;
        else
            sum += term1;
    }

    /* Division by (n+1) is exact according to the formula. */
    sum /= (seq_t)(n + 1);

    return sum;
}
