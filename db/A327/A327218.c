/*
 * A327218  Positions of 1's in {A327216(n) : n > 0}.
 * https://oeis.org/A327218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th positive k with A327216(k) = 1, where A327216(k) =
 *            floor((2k+3)*r) - floor((k+3)*r) - floor(k*r), r =
 *            (1+sqrt(5))/2; found by linear scan
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 8646 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static uint64_t A327218_isqrt(uint64_t x)
{
    uint64_t s = (uint64_t)sqrt((double)x);
    while (s * s > x) s--;
    while ((s + 1) * (s + 1) <= x) s++;
    return s;
}

/* floor(k*phi) = floor((k + floor(sqrt(5k^2))) / 2) */
static uint64_t A327218_fphi(uint64_t k)
{
    return (k + A327218_isqrt(5 * k * k)) / 2;
}

seq_t a(idx_t n)
{
    uint64_t count = 0;
    uint64_t k = 0;
    for (;;) {
        k++;
        uint64_t v = A327218_fphi(2 * k + 3) - A327218_fphi(k + 3) - A327218_fphi(k);
        if (v == 1) {
            count++;
            if (count == (uint64_t)n) return (seq_t)k;
        }
    }
}
