/*
 * A364736  G.f. satisfies A(x) = 1 + x*A(x) / (1 + x*A(x)^3).
 * https://oeis.org/A364736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1/n) * Sum_{k=0..n-1} (-1)^k * binomial(n,k) *
 *            binomial(n+2*k,n-1-k)
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 18, i64 n <= 28, i128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1; a(n) defined for n >= 0. */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    for (idx_t k = 0; k < n; ++k)
    {
        seq_t term1 = binomial(n, k);
        term1 *= binomial(n + 2 * k, n - 1 - k);

        if (k & 1)
            sum -= term1;
        else
            sum += term1;
    }

    return sum / (seq_t)n;
}
