/*
 * A370509  Expansion of Sum_{k>=0} k! * ( x * (1+x^2) )^k.
 * https://oeis.org/A370509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (n-2*k)! * binomial(n-2*k, k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-01)
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
    seq_t sum = 0;
    idx_t maxk = n / 3;            /* floor(n/3) */
    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t m   = n - 2 * k;     /* m = n - 2*k  */
        seq_t term = factorial(m) * binomial(m, k);
        sum += term;
    }
    return sum;
}
