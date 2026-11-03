/*
 * A331726  Expansion of e.g.f. -LambertW(-x/(1 - x)) / (1 - x).
 * https://oeis.org/A331726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(n,k)^2 * k! * (n - k)^(n - k - 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 175 (2026-11-01)
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
        return (seq_t)0;

    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k < n; ++k)
    {
        seq_t bin = binomial(n, k);            /* binomial(n,k) */
        seq_t term = bin * bin;                /* bin^2 */
        term *= factorial(k);                 /* * k! */
        seq_t base = (seq_t)(n - k);           /* n - k */
        seq_t exp  = (seq_t)(n - k - 1);       /* n - k - 1 */
        term *= ipow(base, exp);            /* * (n - k)^(n - k - 1) */
        sum += term;
    }
    return sum;
}
