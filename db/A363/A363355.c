/*
 * A363355  E.g.f. satisfies A(x) = exp(x * A(x) * (1 + x * A(x)^2)).
 * https://oeis.org/A363355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (2*n-k+1)^(k-1) * binomial(k,n-k)/k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes of required utilities */
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
    if (n == 0) return (seq_t)1;

    /* precompute n! */
    seq_t fact_n = factorial(n);

    seq_t sum     = 0;
    seq_t factor  = fact_n;          /* n! / 1!  */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* binomial coefficient bin(k, n-k) */
        seq_t bin = binomial(k, n - k);

        /* base = 2*n - k + 1 */
        seq_t base = (seq_t)(2 * (seq_t)n - (seq_t)k + (seq_t)1);

        /* exponent = k-1 */
        seq_t exp = (seq_t)(k - 1);

        /* pow = base^(k-1) */
        seq_t pow = ipow(base, exp);

        /* term = pow * bin * (n! / k!)  */
        seq_t term = pow * bin * factor;
        sum += term;

        /* update factor for next k (divide by k+1) */
        if (k < n)
            factor = factor / ((seq_t)(k + 1));
    }

    return sum;
}
