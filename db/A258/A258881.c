/*
 * A258881  a(n) = n + the sum of the squared digits of n.
 * https://oeis.org/A258881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + sum of squared digits of n
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 62 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Helper: sum of squares of decimal digits of a 64‑bit integer. */
static uint64_t digit_square_sum(uint64_t x)
{
    uint64_t sum = 0;
    while (x > 0) {
        uint64_t d = x % 10;
        sum += d * d;
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    return (seq_t)n + (seq_t)digit_square_sum(n);
}
