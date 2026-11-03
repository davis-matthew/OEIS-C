/*
 * A380722  E.g.f. A(x) satisfies A(x) = exp(x * A(x)^2 * (1 - x*A(x))) / (1 -
 *          x*A(x))^2.
 * https://oeis.org/A380722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (n+k+1)^(k-1) * binomial(3*n+1, n-k)/k!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n)
{
    /* Base case n = 0 */
    if (n == 0) {
        return 1;
    }

    seq_t n_seq = (seq_t)n;               /* sequence type for n */
    seq_t fact_n = factorial(n);          /* n! */
    seq_t Q = fact_n;                     /* Q_k = n! / k!  (k=0) */
    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        /* binomial(3*n+1, n-k) */
        seq_t bin = binomial(3 * n + 1, n - k);

        /* intermediate product Q * bin */
        seq_t term = Q * bin;

        if (k == 0) {
            /* (n+1)^(−1) is handled by integer division */
            term = term / (n_seq + 1);
        } else {
            /* (n+k+1)^(k-1) */
            seq_t pow_exp = (seq_t)(k - 1);
            seq_t pow_base = n_seq + (seq_t)(k + 1);
            seq_t pow_term = ipow(pow_base, pow_exp);
            term = term * pow_term;
        }

        sum = sum + term;

        /* Prepare Q for k+1: divide by (k+1) */
        if (k < n) {
            Q = Q / (seq_t)(k + 1);
        }
    }

    return sum;
}
