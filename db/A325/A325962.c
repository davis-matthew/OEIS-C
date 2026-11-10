/*
 * A325962  a(1) = 1; for n > 1, a(n) is the largest k <= 1+A046666(n) such
 *          that n-k and n-(sigma(n)-k) are relatively prime, or -1 if no such
 *          nonnegative k exists.
 * https://oeis.org/A325962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; a(n) = max{k : 0<=k<=1+A046666(n), gcd(n-k,
 *            |n-sigma(n)+k|)=1}, else -1 (search downward from
 *            k=1+A046666(n))
 * Range:     u8 n <= 257, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 107 terms through n = 9190 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A325962_gcd(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static uint64_t A325962_sigma(uint64_t n)
{
    uint64_t s = 0, d;
    for (d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            s += d;
            if (n / d != d) s += n / d;
        }
    }
    return s;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n, spf = 0, d, s, kmax;
    int64_t k;
    if (N <= 1) return (seq_t)1;
    for (d = 2; d * d <= N; ++d) {
        if (N % d == 0) { spf = d; break; }
    }
    if (spf == 0) spf = N;
    kmax = 1 + (N - spf);
    s = A325962_sigma(N);
    for (k = (int64_t)kmax; k >= 0; --k) {
        uint64_t x = N - (uint64_t)k;
        int64_t diff = (int64_t)N - (int64_t)s + k;
        uint64_t y = diff >= 0 ? (uint64_t)diff : (uint64_t)(-diff);
        if (A325962_gcd(x, y) == 1) return (seq_t)k;
    }
    return (seq_t)0 - (seq_t)1;
}
