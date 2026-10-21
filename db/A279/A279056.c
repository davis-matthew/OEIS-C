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
 * Helpers:   _floor_isqrt_u64, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A279056: count of representations n = w^2 + x^2 + y^2 + z^2
   with w>=1, x,y,z>=0 and x^3 + 4*y*z*(y-z) a perfect square. */

#include <stdint.h>
#include "../utils/floor_sqrt.h"
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t count = 0;
    idx_t wmax = _floor_isqrt_u64(n);
    for (idx_t w = 1; w <= wmax; ++w) {
        idx_t rem1 = n - (w * w);
        idx_t xmax = _floor_isqrt_u64(rem1);
        for (idx_t x = 0; x <= xmax; ++x) {
            idx_t rem2 = rem1 - (x * x);
            idx_t ymax = _floor_isqrt_u64(rem2);
            for (idx_t y = 0; y <= ymax; ++y) {
                idx_t rem3 = rem2 - (y * y);
                idx_t z = _floor_isqrt_u64(rem3);
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
