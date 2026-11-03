/*
 * A357827  Number of automorphisms of the n-folded cube graph.
 * https://oeis.org/A357827
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2^(n-1) * n! for all n ≥ 3, n ≠ 4; special cases: a(2) =
 *            2, a(4) = 1152
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 16 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"

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
    if (n == 2)
        return (seq_t)2;
    if (n == 4)
        return (seq_t)1152;

    /* a(n) = 2^(n-1) * n! for n ≥ 3 and n ≠ 4 */
    seq_t fact = factorial(n);
    seq_t p2   = pow2(n - 1);
    return fact * p2;
}
