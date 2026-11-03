/*
 * A381559  Number of minimum dominating sets in the n-triangular honeycomb
 *          bishop graph.
 * https://oeis.org/A381559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n%2==0) ? (n/2)! : n * ((n+1)/2)!
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-01)
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
    if (n % 2 == 0) {
        return factorial(n / 2);
    } else {
        seq_t f = factorial((n + 1) / 2);
        return (seq_t)n * f;
    }
}
