/*
 * A327176  Positions of 1's in {A327174(n) : n > 0}.
 * https://oeis.org/A327176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k-th... i.e. a(n) = n-th positive k with A327174(k) = 1,
 *            where A327174(k) = floor((2k+1)*phi) - floor((k+1)*phi) -
 *            floor(k*phi); search by scanning k
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 8074 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static uint64_t A327176_isqrt(__uint128_t x)
{
    uint64_t r = (uint64_t)sqrtl((long double)x);
    while ((__uint128_t)r * r > x) r--;
    while ((__uint128_t)(r + 1) * (r + 1) <= x) r++;
    return r;
}

/* floor(k*phi) exactly */
static uint64_t A327176_fphi(uint64_t k)
{
    return (k + A327176_isqrt((__uint128_t)5 * k * k)) / 2;
}

seq_t a(idx_t n)
{
    uint64_t cnt = 0, k = 0;
    uint64_t prev = 0; /* floor(k*phi) for k */
    uint64_t target = (uint64_t)n;
    while (cnt < target) {
        k++;
        uint64_t fk = A327176_fphi(k);
        uint64_t fk1 = A327176_fphi(k + 1);
        uint64_t f2 = A327176_fphi(2 * k + 1);
        (void)prev;
        if (f2 - fk1 - fk == 1) cnt++;
    }
    return (seq_t)k;
}
