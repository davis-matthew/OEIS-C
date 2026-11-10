/*
 * A327207  Positions of 1's in {A327205(n) : n > 0}.
 * https://oeis.org/A327207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: a(n) = n-th k >= 1 with A327205(k) = 1, where A327205(k)
 *            = floor((2k+2)*sqrt(2)) - floor((k+2)*sqrt(2)) -
 *            floor(k*sqrt(2))
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 8748 (2026-11-08)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u64.h"

/* floor(m*sqrt(2)) computed exactly as isqrt(2*m^2) */
static uint64_t A327207_fl(uint64_t m)
{
    return isqrt_u64(2 * m * m);
}

seq_t a(idx_t n)
{
    uint64_t count = 0;
    uint64_t k = 0;
    while (count < (uint64_t)n)
    {
        k++;
        uint64_t v = A327207_fl(2 * k + 2);
        uint64_t w = A327207_fl(k + 2) + A327207_fl(k);
        if (v == w + 1)
            count++;
    }
    return (seq_t)k;
}
