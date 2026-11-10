/*
 * A395101  Indices k such that A080764(k) = 1.
 * https://oeis.org/A395101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(sqrt(2*n^2)) - 1 = ceil(n*sqrt(2)) - 2
 * Range:     u8 n <= 11, u16 n <= 676, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9794 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A395101_isqrt(seq_t v)
{
    if (v == 0) return (seq_t)0;
    /* start above sqrt(v): v = 2*n^2, so 2*n is a valid upper bound, but
       the helper is general, so find a safe start by doubling */
    seq_t x = 1;
    while (x * x < v) x = x * 2;
    /* x >= sqrt(v); Newton iteration from above */
    for (;;) {
        seq_t y = (x + v / x) / 2;
        if (y >= x) break;
        x = y;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    seq_t r = A395101_isqrt(m * m * 2);
    return r - 1;
}
