/*
 * A344019  A tight upper bound on the order of a finite subgroup of the
 *          collineation group of the free projective plane F_n.
 * https://oeis.org/A344019
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24 (n=4), 12 (n=5), 120 (n=6), 2*(n-2)! (n>=7)
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 26 (2026-11-01)
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
    if (n == 4u) return (seq_t)24;
    if (n == 5u) return (seq_t)12;
    if (n == 6u) return (seq_t)120;
    if (n >= 7u) {
        seq_t f = factorial(n - 2u);   /* (n-2)! */
        return f * (seq_t)2;
    }
    /* For n < 4 the sequence is not defined – return 0 */
    return 0;
}
