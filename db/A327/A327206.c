/*
 * A327206  Positions of 0's in {A327205(n) : n > 0}.
 * https://oeis.org/A327206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k >= 1 with floor((2k+2)*sqrt(2)) -
 *            floor((k+2)*sqrt(2)) - floor(k*sqrt(2)) = 0 (A327205(k)=0);
 *            found by linear scan
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 8953 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static uint64_t A327206_fl(uint64_t k)
{
    uint64_t x = 2 * k * k;
    uint64_t s = (uint64_t)sqrt((double)x);
    while (s * s > x) s--;
    while ((s + 1) * (s + 1) <= x) s++;
    return s;
}

seq_t a(idx_t n)
{
    uint64_t cnt = 0, k = 0;
    while (cnt < (uint64_t)n)
    {
        k++;
        uint64_t v = A327206_fl(2 * k + 2);
        uint64_t w = A327206_fl(k + 2) + A327206_fl(k);
        if (v == w) cnt++;
    }
    return (seq_t)k;
}
