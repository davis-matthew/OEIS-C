/*
 * A327203  Positions of 0's in {A327202(n) : n > 0}.
 * https://oeis.org/A327203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k >= 1 with A327202(k) = 0, where A327202(k) =
 *            floor((2k+2)*phi) - floor((k+2)*phi) - floor(k*phi), phi =
 *            (1+sqrt(5))/2; computed by linear scan using exact floor(k*phi)
 *            = floor((k + isqrt(5k^2))/2)
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9051 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static uint64_t A327203_isqrt(unsigned __int128 x)
{
    uint64_t s = (uint64_t)sqrt((double)x);
    while ((unsigned __int128)s * s > x) s--;
    while ((unsigned __int128)(s + 1) * (s + 1) <= x) s++;
    return s;
}

static long long A327203_fphi(uint64_t k)
{
    unsigned __int128 v = (unsigned __int128)k * k * 5u;
    uint64_t r = A327203_isqrt(v);
    return (long long)((k + r) / 2);
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t k = 0;
    while (count < n) {
        k++;
        long long t = A327203_fphi(2 * k + 2) - A327203_fphi(k + 2) - A327203_fphi(k);
        if (t == 0) count++;
    }
    return (seq_t)k;
}
