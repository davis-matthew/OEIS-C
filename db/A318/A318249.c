/*
 * A318249  a(n) = (n - 1)! * d(n), where d(n) = number of divisors of n
 *          (A000005).
 * https://oeis.org/A318249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * d(n)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 445 (2026-11-01)
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
    seq_t f = factorial(n-1);          /* (n-1)!  */
    uint64_t d = num_divisors((uint64_t)n);  /* d(n) */
    return f * (seq_t)d;               /* (n-1)! * d(n) */
}
