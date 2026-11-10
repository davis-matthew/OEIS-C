/*
 * A327224  Positions of 1's in {A327222(k) : n > 0}.
 * https://oeis.org/A327224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k >= 1 with A327222(k) = 1, where A327222(k) =
 *            floor((2k+4)*sqrt(2)) - floor((k+4)*sqrt(2)) - floor(k*sqrt(2));
 *            found by a linear scan using integer square roots
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9626 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A327224_isqrt(uint64_t x)
{
    uint64_t res = 0, bit = (uint64_t)1 << 62;
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= res + bit) {
            x -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

static uint64_t A327224_fl(uint64_t m)
{
    return A327224_isqrt(2 * m * m);
}

seq_t a(idx_t n)
{
    uint64_t count = 0;
    uint64_t k = 0;
    uint64_t target = (uint64_t)n;
    while (count < target) {
        k++;
        uint64_t v = A327224_fl(2 * k + 4) - A327224_fl(k + 4) - A327224_fl(k);
        if (v == 1) count++;
    }
    return (seq_t)k;
}
