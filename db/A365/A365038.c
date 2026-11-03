/*
 * A365038  E.g.f. satisfies A(x) = exp(x * (1 + x)/A(x)).
 * https://oeis.org/A365038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n! * Σ_{k=0..n} (-k+1)^(k-1) * binomial(k, n-k)/k!
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 19, i128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-01)
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
    if (n == 0)
        return (seq_t)1;

    seq_t result = 0;

    /* n! */
    seq_t fact_n = factorial(n);

    /* first k with a non‑zero binomial term: ceil(n/2) */
    idx_t k_start = (n + 1) / 2;

    /* P = n! / k_start!  (will be updated for subsequent k) */
    seq_t fact_k = factorial(k_start);
    seq_t P = fact_n / fact_k;

    for (idx_t k = k_start; k <= n; ++k)
    {
        /* (-k+1)^(k-1) (0^0 treated as 1 for k==1) */
        seq_t term_power;
        if (k == 1)
            term_power = (seq_t)1;
        else
            term_power = ipow(-(seq_t)(k - 1), (seq_t)(k - 1));

        /* binomial coefficient */
        seq_t bin = binomial(k, n - k);

        /* accumulate term: P * term_power * bin */
        seq_t term = P * term_power * bin;
        result += term;

        /* prepare P for next k: P_next = P / (k+1) */
        if (k < n)
            P /= (seq_t)(k + 1);
    }

    return result;
}
