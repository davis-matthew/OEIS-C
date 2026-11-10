/*
 * A334866  a(0) = 1, and then after, a(2n) = a(n)^2, a(2n+1) = A334747(a(n)).
 * https://oeis.org/A334866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(0) = 1, a(2n) = a(n)^2, a(2n+1) = A334747(a(n)); computed on
 *            prime-exponent vectors (square doubles exponents; A334747 flips
 *            the lowest even-exponent prime up and decrements smaller ones)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 63 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    const int K = 90;
    uint64_t primes[90];
    uint64_t e[90];
    int cnt = 0;
    for (uint64_t c = 2; cnt < K; ++c) {
        bool ok = true;
        for (int j = 0; j < cnt && primes[j] * primes[j] <= c; ++j)
            if (c % primes[j] == 0) { ok = false; break; }
        if (ok) primes[cnt++] = c;
    }
    for (int i = 0; i < K; ++i) e[i] = 0;

    int top = 63;
    while (top >= 0 && !((n >> top) & 1ULL)) --top;
    for (int b = top; b >= 0; --b) {
        if ((n >> b) & 1ULL) {
            int k = 0;
            while (k < K && (e[k] & 1ULL)) ++k;
            if (k >= K) break;
            for (int j = 0; j < k; ++j) e[j] -= 1;
            e[k] += 1;
        } else {
            for (int i = 0; i < K; ++i) e[i] *= 2;
        }
    }

    seq_t result = 1;
    for (int i = 0; i < K; ++i) {
        if (e[i] == 0) continue;
        seq_t base = (seq_t)primes[i];
        uint64_t ex = e[i];
        seq_t r = 1;
        while (ex) {
            if (ex & 1ULL) r = r * base;
            ex >>= 1;
            if (ex) base = base * base;
        }
        result = result * r;
    }
    return result;
}
