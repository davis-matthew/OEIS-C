/*
 * A344000  Indices k such that A011772(k) is even.
 * https://oeis.org/A344000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A011772(k) is even; found by scanning k
 *            upward and computing A011772(k) as the least m>=1 with m ≡ 0 or
 *            -1 (mod each prime power of 2k) via CRT
 * Range:     u8 n <= 157, u16 n <= 9196, u32 n <= 9196, u64 n <= 9196,
 *            u128 n <= 9196, big any n
 * Validated: standard, pass, 91 terms through n = 8257 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static long long A344000_inv(long long a, long long m)
{
    long long g = m, x = 0, y = 1, aa = a % m;
    while (aa != 0) {
        long long qt = g / aa;
        long long t = g - qt * aa; g = aa; aa = t;
        t = x - qt * y; x = y; y = t;
    }
    x %= m;
    if (x < 0) x += m;
    return x;
}

static uint64_t A344000_A011772(uint64_t k)
{
    uint64_t N = 2 * k, t = N;
    uint64_t q[24];
    int c = 0;
    for (uint64_t p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            uint64_t pp = 1;
            while (t % p == 0) { t /= p; pp *= p; }
            q[c++] = pp;
        }
    }
    if (t > 1) q[c++] = t;
    uint64_t best = N - 1;
    for (uint32_t mask = 0; mask < (1u << c); mask++) {
        uint64_t x = 0, M = 1;
        for (int i = 0; i < c; i++) {
            uint64_t r = ((mask >> i) & 1) ? q[i] - 1 : 0;
            uint64_t d = (r + q[i] - x % q[i]) % q[i];
            uint64_t iv = (uint64_t)A344000_inv((long long)(M % q[i]), (long long)q[i]);
            uint64_t s = (uint64_t)((unsigned __int128)d * iv % q[i]);
            x += M * s;
            M *= q[i];
        }
        if (x > 0 && x < best) best = x;
    }
    return best;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t k = 0;
    while (count < n) {
        k++;
        if ((A344000_A011772(k) & 1u) == 0) count++;
    }
    return (seq_t)k;
}
