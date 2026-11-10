/*
 * A356209  a(n) is the position of the latest occurrence of n in A133388.
 * https://oeis.org/A356209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest N = n^2 + j^2 (0 <= j <= n) such that A133388(N)
 *            = n; computed by search from j = n downward (a(n) = 2*n^2 when n
 *            is not in A009003)
 * Range:     u8 n <= 11, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A356209_isqrt(uint64_t x)
{
    uint64_t lo = 0, hi = (x >> 1) + 1, ans = 0;
    if (hi > 4294967295ULL) hi = 4294967295ULL;
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        if (mid * mid <= x) { ans = mid; lo = mid + 1; }
        else { if (mid == 0) break; hi = mid - 1; }
    }
    return ans;
}

/* true if some m > k has N - m^2 a perfect square */
static int A356209_has_larger(uint64_t N, uint64_t k)
{
    uint64_t top = A356209_isqrt(N);
    for (uint64_t m = k + 1; m <= top; ++m) {
        uint64_t rem = N - m * m;
        uint64_t r = A356209_isqrt(rem);
        if (r * r == rem) return 1;
    }
    return 0;
}

seq_t a(idx_t n)
{
    uint64_t k = (uint64_t)n;
    uint64_t j = k;
    for (;;) {
        uint64_t N = k * k + j * j;
        if (!A356209_has_larger(N, k)) return (seq_t)N;
        if (j == 0) break;
        --j;
    }
    return (seq_t)(k * k);
}
