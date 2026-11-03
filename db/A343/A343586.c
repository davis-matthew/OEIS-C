/*
 * A343586  a(n) = the sum of all the multiples of 2 or 5 less than or equal
 *          to 10^n.
 * https://oeis.org/A343586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*10^(2*n-1) + 5*10^(n-1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 473 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* n >= 1 */
    seq_t part1 = pow10((idx_t)(2*n - 1));
    part1 *= (seq_t)3;
    seq_t part2 = pow10((idx_t)(n - 1));
    part2 *= (seq_t)5;
    return part1 + part2;
}
