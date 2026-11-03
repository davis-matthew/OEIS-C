/*
 * A260076  Cyclotomic polynomial value Phi(9,n!).
 * https://oeis.org/A260076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^6 + (n!)^3 + 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 99 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Cyclotomic polynomial value Φ₉(n!) with Φ₉(x) = x⁶ + x³ + 1 */

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
    /* Compute n! */
    seq_t f = factorial(n);

    /* Compute (n!)^6 and (n!)^3 using the provided pow_seq helper */
    seq_t high = ipow_i32(f, 6);
    seq_t mid  = ipow_i32(f, 3);

    /* Return the sum plus 1 */
    return high + mid + (seq_t)1;
}
