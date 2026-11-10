/*
 * A255507  First differences of A255437.
 * https://oeis.org/A255507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A255437(n+1) - A255437(n)
 * Range:     i8 n <= 151, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 98 terms through n = 9585 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static inline idx_t A255437_isqrt(idx_t n)
{
    idx_t low = 0;
    idx_t high = (idx_t)1 << 32;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid <= n / mid) low = mid;
        else high = mid - 1;
    }
    return low;
}

static inline seq_t A255437_C(idx_t m)
{
    idx_t s = A255437_isqrt(m);
    seq_t part1 = (seq_t)m;
    seq_t part2 = (seq_t)s * ((seq_t)s - 1) / 2;
    return part1 + part2;
}

static seq_t A255437_a(idx_t n)
{
    if (n == 0) return 0;
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (A255437_C(mid) >= (seq_t)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;
    idx_t s = A255437_isqrt(m);
    seq_t prevC = A255437_C(m - 1);
    seq_t offset = (seq_t)n - prevC;
    if (s * s == m) {
        return 2 * offset - 1;
    } else {
        return (seq_t)m;
    }
}

seq_t a(idx_t n)
{
    return A255437_a(n + 1) - A255437_a(n);
}
