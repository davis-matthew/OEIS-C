/*
 * A380914  E.g.f. A(x) satisfies A(x) = exp(x / (1 - x*A(x))) / (1 - x*A(x)).
 * https://oeis.org/A380914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (n-k+1)^(k-1) * binomial(2*n-k, n-k)/k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-01)
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
    /* Special case n = 0 */
    if (n == 0)
        return 1;

    seq_t fact_n = factorial(n);           /* n!                    */
    seq_t B      = binomial(2*n, n);        /* B = binom(2n, n)      */

    /* k = 0 term: n! * binom(2n, n)/(n+1) */
    seq_t sum = fact_n * (B / (n + 1));

    seq_t F = fact_n;   /* n! / k!  (k = 0 here) */

    /* iterate for k = 1 .. n */
    for (idx_t k = 1; k <= n; ++k) {
        /* Update B to binom(2n-k, n-k) */
        B = B * (n - (k - 1));          /* multiply by (n - (k-1)) */
        B = B / (2*n - (k - 1));        /* divide by (2n - (k-1)) */

        /* Compute (n-k+1)^(k-1) */
        seq_t base  = (seq_t)(n - k + 1);
        seq_t power = ipow(base, (seq_t)(k - 1));

        /* term = binom * (n!/k!) * power */
        seq_t term = B * F * power;
        sum += term;

        /* Prepare F for next iteration: divide by (k+1) */
        F /= (k + 1);
    }

    return sum;
}
