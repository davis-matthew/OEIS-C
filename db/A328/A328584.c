/*
 * A328584  Least common multiple of n and A276086(n).
 * https://oeis.org/A328584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(n, A276086(n)) = n * A276086(n) / gcd(n, A276086(n));
 *            a(0) = 0
 * Range:     u8 n <= 10, u16 n <= 27, u32 n <= 172, u64 n <= 1677,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9758 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A328584_nextp(uint64_t p)
{
    uint64_t c = p + 1;
    for (;; ++c) {
        bool ok = true;
        for (uint64_t d = 2; d * d <= c; ++d) {
            if (c % d == 0) { ok = false; break; }
        }
        if (ok && c >= 2) return c;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t result = (seq_t)n;
    uint64_t rem = (uint64_t)n;
    uint64_t p = 2;
    while (rem > 0) {
        uint64_t digit = rem % p;
        rem /= p;
        if (digit > 0) {
            uint64_t v = 0;
            uint64_t t = (uint64_t)n;
            while (t % p == 0) { t /= p; ++v; }
            uint64_t m = (digit > v) ? (digit - v) : 0;
            for (uint64_t i = 0; i < m; ++i) result *= (seq_t)p;
        }
        p = A328584_nextp(p);
    }
    return result;
}
