/*
 * A378046  E.g.f. satisfies A(x) = (1+x) * exp(x * A(x)^3 / (1+x)^2).
 * https://oeis.org/A378046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (3*k+1)^(k-1) * C(k+1, n-k) / k!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) exactly using integer arithmetic. */

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
    /* Pre‑compute n! once – this is the common factor in every term. */
    seq_t fact_n = factorial(n);

    seq_t sum = 0;
    /* fact_ratio will hold n! / k! for the current k. */
    seq_t fact_ratio = fact_n;

    for (idx_t k = 0; k <= n; ++k) {
        /* (3*k+1)^(k-1), with the convention that 1^(-1)=1 for k=0. */
        seq_t pow_term;
        if (k == 0 || k == 1) {
            pow_term = 1;
        } else {
            pow_term = ipow((seq_t)(3 * k + 1), (seq_t)(k - 1));
        }

        seq_t bin = binomial(k + 1, n - k);   /* C(k+1, n-k) */

        /* term = (n! / k!) * pow_term * bin */
        seq_t term = fact_ratio * pow_term * bin;
        sum += term;

        /* Update fact_ratio for the next k (division is exact). */
        if (k < n) {
            fact_ratio = fact_ratio / (seq_t)(k + 1);
        }
    }

    return sum;
}
