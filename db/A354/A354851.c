/*
 * A354851  a(n) = (n-1)! * Sum_{d|n} d^(n/d).
 * https://oeis.org/A354851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * Sum_{d|n} d^(n/d) for n >= 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
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
    /* n is guaranteed to be >= 1 by the caller (offset 1) */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            seq_t term1 = ipow((seq_t)d1, (seq_t)(n / d1));
            sum += term1;

            if (d1 != d2) {
                seq_t term2 = ipow((seq_t)d2, (seq_t)(n / d2));
                sum += term2;
            }
        }
    }
    seq_t fact = factorial(n - 1);
    return fact * sum;
}
