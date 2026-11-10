/*
 * A327307  Positions of 0's in {A327306(n) : n > 0}.
 * https://oeis.org/A327307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th positive k with floor(3*k*sqrt(6)) -
 *            3*floor(k*sqrt(6)) = 0 (A327306(k)=0); found by a linear scan
 *            with exact integer square roots
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9703 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static uint64_t A327307_isqrt(unsigned __int128 x)
{
    uint64_t r = (uint64_t)sqrt((double)x);
    while ((unsigned __int128)r * r > x) r--;
    while ((unsigned __int128)(r + 1) * (r + 1) <= x) r++;
    return r;
}

static int A327307_zero(uint64_t k)
{
    unsigned __int128 k2 = (unsigned __int128)k * k;
    uint64_t f1 = A327307_isqrt(6 * k2);
    uint64_t f3 = A327307_isqrt(54 * k2);
    return f3 == 3 * f1;
}

seq_t a(idx_t n)
{
    static idx_t last_n = 0;
    static uint64_t last_k = 0;
    idx_t cnt = 0;
    uint64_t k = 0;
    if (last_n != 0 && last_n <= n) {
        cnt = last_n;
        k = last_k;
    }
    while (cnt < n) {
        k++;
        if (A327307_zero(k)) cnt++;
    }
    last_n = n;
    last_k = k;
    return (seq_t)k;
}
