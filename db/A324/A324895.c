/*
 * A324895  Largest proper divisor of A276086(n); a(0) = 1.
 * https://oeis.org/A324895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A032742(A276086(n)) = A276086(n) / (smallest prime factor
 *            of A276086(n)); a(0) = 1
 * Range:     u8 n <= 20, u16 n <= 86, u32 n <= 418, u64 n <= 4408,
 *            u128 n <= 30030, big any n
 * Validated: standard, pass, 91 terms through n = 9297 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A324895_next_prime(uint64_t p)
{
    uint64_t c = p + 1;
    if (c <= 2) return 2;
    if ((c & 1) == 0) c++;
    for (;; c += 2) {
        bool ok = true;
        for (uint64_t d = 3; d <= c / d; d += 2) {
            if (c % d == 0) { ok = false; break; }
        }
        if (ok) return c;
    }
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p = 2;
    bool first = true;
    while (m > 0) {
        uint64_t d = m % p;
        m /= p;
        if (d > 0) {
            if (first) { d--; first = false; }
            for (uint64_t i = 0; i < d; i++)
                result *= (seq_t)p;
        }
        p = A324895_next_prime(p);
    }
    return result;
}
