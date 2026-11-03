/*
 * A295406  a(n) = n! * Laguerre(n, 2*n, -n).
 * https://oeis.org/A295406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} binomial(3*n, n-k) * n^k / k!
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 292 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

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
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* factorial(n) */
    seq_t fact = factorial(n);

    /* initial term for k=0:
       term0 = binomial(3*n, n) * n! */
    seq_t term = binomial(3 * n, n) * fact;
    seq_t sum  = term;

    /* iterate k = 1 .. n */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* term_k = term_{k-1} * n * (n-(k-1)) /
                    ( k * (2*n + k) )  */
        seq_t numer   = (term * (seq_t)n) * (seq_t)(n - (k - 1));
        seq_t denom   = (seq_t)k * (seq_t)(2 * n + k);
        term = numer / denom;
        sum += term;
    }

    return sum;
}
