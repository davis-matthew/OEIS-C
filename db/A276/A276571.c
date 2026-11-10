/*
 * A276571  After a(0)=0, each n occurs A260734(n) times.
 * https://oeis.org/A276571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; for n>=1, a(n)=m where m is the least index with
 *            A260734(1)+...+A260734(m) >= n (each m repeated A260734(m)
 *            times). Computed by direct iteration
 * Range:     u8 n <= 10028, u16 n <= 10028, u32 n <= 10028, u64 n <= 10028,
 *            u128 n <= 10028, big any n
 * Validated: standard, pass, 99 terms through n = 9746 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A276571_isqrt(uint64_t x)
{
    uint64_t lo = 0, hi = 1;
    while (hi <= x / hi && hi < 4294967296ULL)
        hi <<= 1;
    if (hi > 4294967295ULL) hi = 4294967296ULL;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (mid <= x / mid) lo = mid + 1;
        else hi = mid;
    }
    return lo - 1;
}

static int A276571_two_squares(uint64_t n)
{
    uint64_t t = n;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) { t /= p; e++; }
            if ((p & 3) == 3 && (e & 1)) return 0;
        }
    }
    if (t > 1 && (t & 3) == 3) return 0;
    return 1;
}

static uint64_t A276571_minsq(uint64_t n)
{
    uint64_t s = A276571_isqrt(n);
    if (s * s == n) return 1;
    if (A276571_two_squares(n)) return 2;
    uint64_t t = n;
    while ((t & 3) == 0) t >>= 2;
    if ((t & 7) == 7) return 4;
    return 3;
}

static uint64_t A276571_A260734(uint64_t m)
{
    uint64_t start = (m + 1) * (m + 1) - 1;
    uint64_t target = m * m - 1;
    uint64_t count = 0;
    while (start > target) {
        start -= A276571_minsq(start);
        ++count;
    }
    return count;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t cum = 0, m = 1;
    for (;;) {
        cum += A276571_A260734(m);
        if ((uint64_t)n <= cum) return (seq_t)m;
        ++m;
    }
}
