/*
 * A365111  G.f. satisfies A(x) = 1 + x / (1 + x*A(x))^4.
 * https://oeis.org/A365111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * binomial(n-k+1,k) * *
 *            binomial(n+3*k-1, n-k) / (n-k+1) (s = 4)
 * Range:     i8 n <= 5, i16 n <= 10, i32 n <= 14, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   binom_my
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_my.h"

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0) return 1;

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        /* term1 = binomial(n-k+1, k) */
        seq_t term1 = binom_my(n - k + 1, k);

        /* term2 = binomial(n + 3*k - 1, n - k) */
        seq_t term2 = binom_my(n + 3 * k - 1, n - k);

        /* denom = n - k + 1 */
        seq_t denom = (seq_t)(n - k + 1);

        /* product / denom */
        seq_t prod = (term1 * term2) / denom;

        /* sign = (-1)^(n-k) */
        if ((n - k) & 1ULL) prod = -prod;

        sum += prod;
    }
    return sum;
}
