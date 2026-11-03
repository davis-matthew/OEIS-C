/*
 * A269889  The number of permutations of 1, 2,..., n such that none of 123,
 *          132, 213, 231, 312, 321 appear in the permutation.
 * https://oeis.org/A269889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! - 6*(n-2)! for n >= 3 with initial values a(0)=1,
 *            a(1)=1, a(2)=2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-01)
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
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)2;

    seq_t fact_n  = factorial(n);     /* n!   */
    seq_t fact_n2 = factorial(n - 2); /* (n-2)! */
    return fact_n - ((seq_t)6 * fact_n2);
}
