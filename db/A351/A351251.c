/*
 * A351251  Denominator of n / A276086(n).
 * https://oeis.org/A351251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = A276086(n)/gcd(n,A276086(n)); with primorial-base digits
 *            d_k of n, a(n) = prod_k prime(k+1)^(d_k - min(d_k,
 *            v_{prime(k+1)}(n))), a(0)=1
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 418, u64 n <= 2308,
 *            u128 n <= 11550, big any n
 * Validated: standard, pass, 84 terms through n = 9514 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    uint64_t primes[24];
    int cnt = 0;
    for (uint64_t c = 2; cnt < 24; ++c) {
        bool ok = true;
        for (int i = 0; i < cnt; ++i) {
            if (primes[i] * primes[i] > c) break;
            if (c % primes[i] == 0) { ok = false; break; }
        }
        if (ok) primes[cnt++] = c;
    }

    seq_t result = (seq_t)1;
    uint64_t w = 1;
    for (int k = 0; k < 24; ++k) {
        if (w > (uint64_t)n) break;
        uint64_t p = primes[k];
        uint64_t d = ((uint64_t)n / w) % p;
        uint64_t v = 0;
        uint64_t m = (uint64_t)n;
        while (m % p == 0) { m /= p; ++v; }
        uint64_t e = (d > v) ? d - v : 0;
        for (uint64_t t = 0; t < e; ++t) result *= (seq_t)p;
        if (w > UINT64_MAX / p) break;
        w *= p;
    }
    return result;
}
