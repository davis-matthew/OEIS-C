/*
 * A325961  a(n) is the least k >= A061228(n)-1 such that n-k and
 *          n-(sigma(n)-k) are relatively prime, or 0 if no such k <= sigma(n)
 *          exists.
 * https://oeis.org/A325961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k >= A061228(n)-1 with gcd(n-k, n-(sigma(n)-k)) =
 *            1, k <= sigma(n), else 0 (direct search)
 * Range:     u8 n <= 246, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 107 terms through n = 9944 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A325961_gcd(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static uint64_t A325961_sigma(uint64_t n)
{
    uint64_t result = 1, m = n, p;
    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            uint64_t term = 1, pw = 1;
            while (m % p == 0) { m /= p; pw *= p; term += pw; }
            result *= term;
        }
    }
    if (m > 1) result *= (m + 1);
    return result;
}

static uint64_t A325961_A061228(uint64_t n)
{
    uint64_t d;
    if (n <= 1) return 2;
    if ((n & 1) == 0) return n + 2;
    for (d = 3; d * d <= n; d += 2)
        if (n % d == 0) return n + d;
    return 2 * n;
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    uint64_t s = A325961_sigma(nn);
    uint64_t k = A325961_A061228(nn) - 1;
    for (; k <= s; k++) {
        uint64_t d1 = (nn >= k) ? nn - k : k - nn;
        uint64_t t = nn + k;
        uint64_t d2 = (t >= s) ? t - s : s - t;
        if (A325961_gcd(d1, d2) == 1)
            return (seq_t)k;
    }
    return (seq_t)0;
}
