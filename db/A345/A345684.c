/*
 * A345684  a(n) = n! * Sum_{k=1..n} k/floor(n/k).
 * https://oeis.org/A345684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} k / floor(n/k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 372 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined for n ≥ 1. For n = 0 the function returns 0. */
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
    /* Handle the offset: the sequence is undefined for n = 0 */
    if (n == 0)
        return 0;

    /* n! */
    seq_t fact = factorial(n);

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t denom = n / k;                     /* floor(n/k) */
        seq_t denom_t = (seq_t)denom;
        seq_t term = (fact / denom_t) * (seq_t)k;
        sum += term;
    }
    return sum;
}
