/*
 * A360697  The sum of the squares of the digits of n, repeated until reaching
 *          a single-digit number.
 * https://oeis.org/A360697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2371, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 117 terms through n = 9627 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A360697: For n ≥ 0, a(n) is the single‑digit result of the
   iterative process “replace n by the sum of the squares of its decimal
   digits” repeated until the result is < 10. */
#include <stdint.h>

/* Compute the sum of the squares of the decimal digits of v.
   Works for all supported seq_t types (uintXX_t or arbitrary‑precision). */
static seq_t sum_sq_digits(seq_t v)
{
    seq_t res  = 0;
    const seq_t ten = 10;
    while (v > 0) {
        seq_t d = v % ten;
        res += d * d;
        v /= ten;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t val = sum_sq_digits((seq_t)n);
    while (val >= 10) {
        val = sum_sq_digits(val);
    }
    return val;
}
