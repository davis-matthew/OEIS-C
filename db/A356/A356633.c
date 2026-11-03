/*
 * A356633  a(n) = n! * Sum_{k=0..floor(n/3)} (n - 3*k)^k/6^k.
 * https://oeis.org/A356633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/3)} (n - 3*k)^k / 6^k
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-01)
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
    seq_t fac = factorial(n);      /* n! */
    seq_t sum = 0;
    seq_t fact_div = fac;          /* n! / 6^k, initially k = 0 */

    for (idx_t k = 0; k <= n / 3; ++k) {
        seq_t base = (seq_t)(n - 3 * k);
        seq_t pow_base = ipow_i32(base, (int)k);
        seq_t term = fact_div * pow_base;
        sum += term;

        /* Prepare divisor for next k; exact because 6^k divides n! */
        fact_div = fact_div / 6;
    }
    return sum;
}
