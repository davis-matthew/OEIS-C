/*
 * A364762  G.f. satisfies A(x) = 1 / (1 + x*(1 + x*A(x))^4).
 * https://oeis.org/A364762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1/(n+1) * ∑_{k=0..n} (-1)^k * binomial(n+1,k) *
 *            binomial(4*k, n-k)
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 15, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute the hypergeometric sum */
    idx_t k_start = (n + 4) / 5;          /* ceil(n/5) */
    seq_t sum = 0;

    for (idx_t k = k_start; k <= n; ++k)
    {
        seq_t term = binomial(n + 1, k) * binomial(4 * k, n - k);
        if (k & 1)
            sum -= term;
        else
            sum += term;
    }

    /* Divide by (n+1); the division is exact */
    return sum / (seq_t)(n + 1);
}
