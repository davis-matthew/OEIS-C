/*
 * A356529  a(n) = (n-1)! * Sum_{d|n} d^(n-d+1).
 * https://oeis.org/A356529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * Sum_{d|n} d^{(n-d+1)}
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

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
    if (n < 1) return 0;                         /* offset starts at 1 */

    seq_t sum = 0;
    for (idx_t d = 1; d <= n; ++d) {
        if (n % d == 0) {
            seq_t exp = (seq_t)(n - d + 1);
            sum += ipow((seq_t)d, exp);
        }
    }

    seq_t fact = factorial(n - 1);
    return fact * sum;
}
