/*
 * A323295  Number of ways to fill a matrix with the first n positive
 *          integers.
 * https://oeis.org/A323295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n) * n! for n > 0, a(0) = 1, where d(n) = A000005(n) is
 *            the number of divisors of n
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/num_divisors.h"

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
    /* Handle the offset explicitly */
    if (n == 0)
        return (seq_t)1;

    /* d(n) – number of positive integer divisors of n */
    seq_t d = (seq_t)num_divisors((uint64_t)n);

    /* n! – factorial of n */
    seq_t f = factorial(n);

    /* Product gives the desired sequence value */
    return d * f;
}
