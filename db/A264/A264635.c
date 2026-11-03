/*
 * A264635  Number of n X 1 arrays of permutations of 0..n*1-1 with rows
 *          nondecreasing modulo 2 and columns nondecreasing modulo 4.
 * https://oeis.org/A264635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=0..3} ⌊(n+i)/4⌋!
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 21, u64 n <= 32, u128 n <= 51,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 62 (2026-11-01)
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
    /* Compute floor((n+i)/4) for i=0..3 */
    idx_t f0 = n/4;
    idx_t f1 = (n+1)/4;
    idx_t f2 = (n+2)/4;
    idx_t f3 = (n+3)/4;

    /* Multiply the corresponding factorials */
    seq_t res = factorial(f0);
    res *= factorial(f1);
    res *= factorial(f2);
    res *= factorial(f3);
    return res;
}
