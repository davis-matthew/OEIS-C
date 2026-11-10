/*
 * A369043  LCM-transform of Blue code (A193231).
 * https://oeis.org/A369043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A014963(A193231(n)), where A193231(n) = XOR over set bits
 *            e of n of (x+1)^e as a GF(2) polynomial, and A014963(m) = p if m
 *            is a power of the prime p, else 1
 * Range:     u8 n <= 255, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 114 terms through n = 9711 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A193231_blue(uint64_t n)
{
    uint64_t res = 0, mask = 1, t = n;
    while (t) {
        if (t & 1)
            res ^= mask;
        mask = (mask << 1) ^ mask;
        t >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    uint64_t m = A193231_blue((uint64_t)n);
    if (m <= 1)
        return (seq_t)1;

    uint64_t spf = 0;
    for (uint64_t i = 2; i <= m / i; ++i) {
        if (m % i == 0) {
            spf = i;
            break;
        }
    }
    if (spf == 0)
        return (seq_t)m;

    uint64_t t = m;
    while (t % spf == 0)
        t /= spf;

    return (t == 1) ? (seq_t)spf : (seq_t)1;
}
