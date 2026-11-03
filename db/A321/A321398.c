/*
 * A321398  a(n) = (-1)^(n+1)*n!* [x^n](log(x + 1)/2 + log(3*x + 1)/6).
 * https://oeis.org/A321398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n-1)! * (3^n + 3)) / 6 for n ≥ 1 a(0) = 0 (offset 0)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 353 (2026-11-01)
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

/* Computes a(n) for A321398 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* (n-1)! */
    seq_t fact = factorial(n - 1);         

    /* 3^n via exponentiation by squaring */
    seq_t pow3 = (seq_t)1;
    seq_t base  = (seq_t)3;
    idx_t  exp  = n;
    while (exp)
    {
        if (exp & 1)
            pow3 = pow3 * base;
        base = base * base;
        exp >>= 1;
    }

    /* (3^n + 3) / 6  (exact division) */
    seq_t mult = (pow3 + (seq_t)3) / (seq_t)6;

    /* Final result */
    return fact * mult;
}
