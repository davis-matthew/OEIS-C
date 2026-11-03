/*
 * A296785  Detour index for the n-transposition graph.
 * https://oeis.org/A296785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * (n!/2 - 1)^2 + (n!/2)^2 * (n! - 1) (n > 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 85 (2026-11-01)
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

/* Computes the OEIS sequence A296785 (detour index for the n‑transposition graph). */
seq_t a(idx_t n)
{
    /* By definition a(1) = 0.  All other n >= 2 use the formula. */
    if (n <= 1)
        return 0;

    /* Compute n! once. */
    seq_t fact = factorial(n);

    /* Since n! is even for n >= 2, division by 2 is exact. */
    seq_t half = fact / 2;

    /* term1 = n! * (half - 1)^2 */
    seq_t tmp   = half - 1;          /* half-1 is non‑negative */
    seq_t term1 = fact * tmp * tmp;

    /* term2 = (half * half) * (n! - 1) */
    seq_t term2 = (half * half) * (fact - 1);

    return term1 + term2;
}
