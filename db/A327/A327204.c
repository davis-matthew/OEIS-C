/*
 * A327204  Positions of 1's in {A327202(n) : n > 0}.
 * https://oeis.org/A327204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k >= 1 with A327202(k) = 1, where A327202(k) =
 *            floor((2k+2)r) - floor((k+2)r) - floor(k*r), r = (1+sqrt(5))/2;
 *            found by linear search, floor(m*r) = (m + isqrt(5m^2))/2
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9248 (2026-11-08)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u64.h"

static uint64_t A327204_fl(uint64_t m)
{
    return (m + isqrt_u64(5ULL * m * m)) / 2;
}

seq_t a(idx_t n)
{
    uint64_t count = 0;
    uint64_t k = 0;
    while (count < (uint64_t)n) {
        k++;
        uint64_t v = A327204_fl(2 * k + 2) - A327204_fl(k + 2) - A327204_fl(k);
        if (v == 1) count++;
    }
    return (seq_t)k;
}
