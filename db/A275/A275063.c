/*
 * A275063  Number of permutations p of [n] such that p(i)-i is a multiple of
 *          eight for all i in [n].
 * https://oeis.org/A275063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{i=0..7} ⌊(n+i)/8⌋!
 * Range:     u8 n <= 15, u16 n <= 21, u32 n <= 29, u64 n <= 43, u128 n <= 65,
 *            big any n
 * Validated: standard, pass, 52 terms through n = 638 (2026-11-01)
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
    seq_t res = 1;
    for (idx_t i = 0; i < 8; ++i) {
        idx_t m = (n + i) / 8;          /* floor((n+i)/8) */
        seq_t f = factorial(m);
        res *= f;
    }
    return res;
}
