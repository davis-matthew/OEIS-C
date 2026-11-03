/*
 * A277168  Coefficients in the series reversion of x*exp(-x^2).
 * https://oeis.org/A277168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n-1, n-1) * (2*n-1)^(n-2) * n! (a(1)=1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 189 (2026-11-01)
 * Helpers:   binomial, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_idx.h"

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
    if (n == 1) return (seq_t)1;                      /* a(1) = 1 */

    idx_t two_n_minus1 = 2 * n - 1;                   /* 2n-1 */
    /* binomial coefficient C(2n-1, n-1) */
    seq_t comb = binomial(two_n_minus1, n - 1);

    /* (2n-1)^(n-2)  (exponent n-2 >= 0 for n>=2) */
    seq_t pow_val = ipow_idx((seq_t)two_n_minus1, n - 2);

    /* n! */
    seq_t fact = factorial(n);

    /* product */
    return comb * pow_val * fact;
}
