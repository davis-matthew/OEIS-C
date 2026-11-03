/*
 * A269221  Factorial of the sum of decimal digits of n.
 * https://oeis.org/A269221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = factorial(digit_sum(n))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 48, u64 n <= 398, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 60 terms through n = 993 (2026-11-01)
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

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
    /* compute sum of decimal digits of n */
    seq_t s = 0;
    uint64_t t = (uint64_t)n;          /* n is idx_t, guaranteed to be uint64_t */
    while (t > 0) {
        s += (seq_t)(t % 10);
        t /= 10;
    }
    /* factorial of that sum */
    return factorial((idx_t)s);
}
