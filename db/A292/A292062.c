/*
 * A292062  Wiener index of the n-transposition graph.
 * https://oeis.org/A292062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n*n! - |Stirling1(n+1,2)|) / 2 * (using
 *            |Stirling1(n+1,2)| = n! * H_n, so the value is * (n! * n! * (n -
 *            H_n)) / 2 * which is implemented as * (n!^2 * n - Σ_{i=1..n}
 *            (n!^2 / i)) / 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 50 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

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
    if (n == 0) /* undefined by OEIS, but return 0 for safety */
        return 0;

    /* n! */
    seq_t fact = factorial(n);

    /* n!^2 */
    seq_t fact2 = fact * fact;

    /* Σ_{i=1..n} (n!^2 / i) */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i)
        sum += fact2 / (seq_t)i;

    /* (n!^2 * n - sum) / 2 */
    seq_t numerator = fact2 * (seq_t)n - sum;
    return numerator / 2;
}
