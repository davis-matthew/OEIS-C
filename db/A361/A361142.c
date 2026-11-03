/*
 * A361142  E.g.f. satisfies A(x) = exp( x*A(x)^2/(1 - x*A(x)) ).
 * https://oeis.org/A361142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (n+k+1)^(k-1) * binomial(n-1, n-k)/k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 311 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computational strategy:
   - Replace k! in the denominator by the integer factor n!/k! which can be
     computed incrementally while iterating over k.
   - For each k the term is binomial(n-1,k-1) * (n+k+1)^(k-1) * (n!/k!).
   - binomial(n-1,k-1) is updated multiplicatively.
   - The power (n+k+1)^(k-1) is computed with the provided big_pow
     routine (exponentiation by squaring).
   - All arithmetic uses the arbitrary‑precision type seq_t, no intermediate
     floating point operations.
*/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

static seq_t fact_upto(idx_t m)
{
    seq_t r = 1;
    for (idx_t i = 2; i <= m; ++i)
        r *= (seq_t)i;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;

    /* binomial(n-1, k-1) starts with k=1 -> value 1 */
    seq_t bin = 1;

    /* factorial(n) is needed once. */
    seq_t fact_n = fact_upto(n);
    /* mult = n! / k! for the current k, start with k=1. */
    seq_t mult = fact_n;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* (n+k+1)^(k-1) */
        seq_t power = ipow((seq_t)(n + k + 1), (seq_t)(k - 1));

        /* accumulate the term */
        sum += bin * power * mult;

        if (k < n)
        {
            /* prepare for the next k */
            mult = mult / (seq_t)(k + 1);                 /* n!/ (k+1)!  */
            bin   = bin * (seq_t)(n - k) / (seq_t)k;     /* binom(n-1,k) */
        }
    }
    return sum;
}
