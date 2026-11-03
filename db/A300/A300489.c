/*
 * A300489  a(n) = n! * [x^n] -log(1 - x)/(1 - n*x).
 * https://oeis.org/A300489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} n^(n-k)/k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

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
        return 0;

    /* n! */
    seq_t fact = factorial(n);

    /* compute n^(n-1) – for k = 1 the exponent is n‑1 */
    seq_t pow = ipow_i32((seq_t)n, n - 1);

    seq_t sum = 0;

    for (idx_t k = 1; k <= n; ++k) {
        /* term = (n! / k) * n^(n-k)  */
        seq_t term = (fact / (seq_t)k) * pow;
        sum += term;

        /* prepare pow for the next k (divide by n) */
        if (k != n)
            pow /= (seq_t)n;
    }

    return sum;
}
