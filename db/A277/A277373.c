/*
 * A277373  a(n) = Sum_{k=0..n} binomial(n,n-k)*n^(n-k)*n!/(n-k)!.
 * https://oeis.org/A277373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Σ_{j=0..n} binomial(n,j) * n^j * n! / j!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 340 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the recurrence T_{j+1} = T_j * (n-j) * n / (j+1)^2, */
/* starting from T_0 = n! . */

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
    /* initial term T_0 = n! */
    seq_t term = factorial(n);
    seq_t sum   = term;

    for (idx_t j = 0; j < n; ++j) {
        idx_t j1 = j + 1;

        /* compute T_{j+1} = T_j * (n-j) * n / (j+1)^2
           using two exact divisions to keep intermediates smaller */
        term = term * (seq_t)(n - j);     /* multiply by (n-j) */
        term = term / (seq_t)j1;          /* divide by (j+1) */
        term = term * (seq_t)n;           /* multiply by n     */
        term = term / (seq_t)j1;          /* divide by (j+1) again */

        sum += term;
    }

    return sum;
}
