/*
 * A344763  a(n) = n - A011772(n).
 * https://oeis.org/A344763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n - A011772(n), where A011772(n) is the smallest m >= 1
 *            with n | m(m+1)/2 (found by search over m < 2n)
 * Range:     i8 n <= 152, i16 n <= 16383, i32 n <= 16383, i64 n <= 16383,
 *            i128 n <= 16383, big any n
 * Validated: standard, pass, 106 terms through n = 7485 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A344763_A011772(idx_t n)
{
    if (n == 0) return 0;
    idx_t limit = 2 * n;
    for (idx_t m = 1; m < limit; ++m) {
        idx_t x = m, y = m + 1;
        if ((x & 1) == 0) x /= 2; else y /= 2;
        unsigned __int128 p = (unsigned __int128)(x % n) * (unsigned __int128)(y % n);
        if ((idx_t)(p % n) == 0) return m;
    }
    return limit - 1;
}

seq_t a(idx_t n)
{
    return (seq_t)n - (seq_t)A344763_A011772(n);
}
