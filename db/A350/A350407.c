/*
 * A350407  a(n) = [x^n] 1/(1 + x + x^2 + x^3 + x^4)^n.
 * https://oeis.org/A350407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Σ_{k=0..n} (-1)^(n-k) * binomial(n-1+k,k) *
 *            binomial(n,5*k)
 * Range:     i8 n <= 6, i16 n <= 13, i32 n <= 23, i64 n <= 46, i128 n <= 90,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        /* binomial(n-1+k, k) */
        seq_t b1 = binomial(n - 1 + k, k);

        /* binomial(n, 5*k) – zero when 5*k > n */
        seq_t b2 = binomial(n, 5 * k);
        if (b2 == 0)          /* optional optimisation, avoids useless multiplication */
            continue;

        seq_t term = b1 * b2;
        if (((n - k) & (idx_t)1))
            term = -term;
        sum += term;
    }
    return sum;
}
