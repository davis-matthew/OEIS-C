/*
 * A327309  Positions of 2's in {A327306(n) : n > 0}.
 * https://oeis.org/A327309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k >= 1 with A327306(k) = 2, where A327306(k) =
 *            floor(3*k*sqrt(6)) - 3*floor(k*sqrt(6)); found by a linear scan
 *            over k
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9358 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned __int128 A327309_isqrt(unsigned __int128 x)
{
    unsigned __int128 lo = 0;
    unsigned __int128 hi = (((unsigned __int128)1 << 64) - 1);
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

static inline int A327309_f(uint64_t k)
{
    unsigned __int128 k2 = (unsigned __int128)k * k;
    unsigned __int128 f1 = A327309_isqrt(6 * k2);
    unsigned __int128 f3 = A327309_isqrt(54 * k2);
    return (int)(f3 - 3 * f1);
}

seq_t a(idx_t n)
{
    uint64_t count = 0;
    uint64_t k = 0;
    while (count < (uint64_t)n) {
        k++;
        if (A327309_f(k) == 2)
            count++;
    }
    return (seq_t)k;
}
