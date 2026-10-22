/*
 * A362735  E.g.f. satisfies A(x) = exp(x + x / A(x)^2).
 * https://oeis.org/A362735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-2*k+1)^(n-1) * binomial(n,k)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 13, i128 n <= 22,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    seq_t binom = 1;                      /* binomial(n,0) */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t base = ((seq_t)(-2)) * (seq_t)k + (seq_t)1;   /* (-2*k+1) */
        seq_t pow_val = pow_int(base, n - 1);               /* (−2k+1)^(n-1) */
        seq_t term = binom * pow_val;
        sum += term;

        if (k < n) {                       /* update for next k */
            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
        }
    }
    return sum;
}
