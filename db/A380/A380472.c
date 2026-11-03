/*
 * A380472  a(n) = gcd_{primes P >= prime(n+1)} Product_{i=1..n} (P^2-i^2).
 * https://oeis.org/A380472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n+2)! * (3/4-(-1)^n/4) (i.e. a(n) = (2n+2)! if n is
 *            odd, a(n) = (2n+2)!/2 if n is even)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
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
    /* compute (2*n+2)! */
    idx_t m = 2*n + 2;
    seq_t fact = factorial(m);

    /* divide by 2 for even n */
    if ((n & 1) == 0)
        fact = fact / 2;

    return fact;
}
