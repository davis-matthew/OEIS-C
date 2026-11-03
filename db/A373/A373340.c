/*
 * A373340  Number of permutations of symmetric group S_n with an odd number
 *          of cycles of length 2 or more.
 * https://oeis.org/A373340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! / 2 + (n-2) * 2^(n-2)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-01)
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
    /* For n = 0 or 1 the only permutation has zero non‑fixed
       point cycles, so the result is 0. */
    if (n < 2)
        return 0;

    /* Compute n! and divide by 2 (integer division). */
    seq_t fact = factorial(n);
    seq_t part1 = fact / 2;

    /* Compute 2^(n-2) and multiply by (n-2). */
    seq_t pow = pow2(n - 2);
    seq_t part2 = (seq_t)(n - 2) * pow;

    /* Sum the two parts to obtain a(n). */
    return part1 + part2;
}
