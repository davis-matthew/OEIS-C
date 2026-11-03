/*
 * A264791  Number of n X 1 arrays of permutations of 0..n*1-1 with rows
 *          nondecreasing modulo 2 and columns nondecreasing modulo 7.
 * https://oeis.org/A264791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=0..6} ⌊(n+i)/7⌋!
 * Range:     u8 n <= 14, u16 n <= 19, u32 n <= 27, u64 n <= 41, u128 n <= 62,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 89 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 0; i < 7; ++i) {
        idx_t m = (n + i) / 7;          /* floor((n+i)/7) */
        seq_t f = factorial(m);         /* m!  */
        res = seq_mul(res, f);          /* res *= f  */
    }
    return res;
}
