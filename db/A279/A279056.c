/*
 * A279056  Number of ways to write n as w^2 + x^2 + y^2 + z^2 with w a
 *          positive integer and x,y,z nonnegative integers such that x^3 +
 *          4*y*z*(y-z) is a square.
 * https://oeis.org/A279056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 80, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9798 (2026-11-01)
 * Helpers:   ipow_i32, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A279056: count of representations n = w^2 + x^2 + y^2 + z^2
   with w>=1, x,y,z>=0 and x^3 + 4*y*z*(y-z) a perfect square. */

#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/isqrt.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t count = 0;
    idx_t wmax = isqrt(n);
    for (idx_t w = 1; w <= wmax; ++w) {
        idx_t rem1 = n - (w * w);
        idx_t xmax = isqrt(rem1);
        for (idx_t x = 0; x <= xmax; ++x) {
            idx_t rem2 = rem1 - (x * x);
            idx_t ymax = isqrt(rem2);
            for (idx_t y = 0; y <= ymax; ++y) {
                idx_t rem3 = rem2 - (y * y);
                idx_t z = isqrt(rem3);
                if (z * z != rem3) continue;
                __int128 val128 = (__int128)x * x * x;
                val128 += (__int128)4 * y * z * ((int64_t)y - (int64_t)z);
                if (val128 < 0) continue;
                seq_t val = (seq_t)val128;
                seq_t sq = floor_sqrt(val);
                if (sq * sq == val) ++count;
            }
        }
    }
    return count;
}
