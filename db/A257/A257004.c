/*
 * A257004  Number of primitive Zagier-reduced indefinite quadratic forms over
 *          the integers in two variables with discriminants D(n) =
 *          A079896(n).
 * https://oeis.org/A257004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (A,B,C), A>0, C>0, B>A+C, B^2-4AC=D,
 *            gcd(A,B,C)=1, D=A079896(n); computed by direct enumeration over
 *            B
 * Range:     u8 n <= 858, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9647 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t zr_isqrt(uint64_t x)
{
    uint64_t lo = 0, hi = x < 4294967295ULL ? x : 4294967295ULL;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        if (mid * mid <= x) lo = mid; else hi = mid - 1;
    }
    return lo;
}

static uint64_t zr_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

seq_t a(idx_t n)
{
    /* find D = A079896(n) */
    uint64_t D = 0;
    idx_t found = 0;
    uint64_t c = 1;
    while (found < n) {
        ++c;
        if ((c & 3) == 0 || (c & 3) == 1) {
            uint64_t r = zr_isqrt(c);
            if (r * r != c) { ++found; D = c; }
        }
    }

    uint64_t s = zr_isqrt(D);
    uint64_t Bmax = (D + 1) / 2;
    seq_t count = 0;

    for (uint64_t B = s + 1; B <= Bmax; ++B) {
        if (((B * B) ^ D) & 1) continue;
        uint64_t N = (B * B - D) / 4;
        if (N == 0) continue;
        for (uint64_t d = 1; d * d <= N; ++d) {
            if (N % d) continue;
            uint64_t e = N / d;
            if (d + e >= B) continue;
            if (zr_gcd(zr_gcd(d, e), B) != 1) continue;
            count += (d == e) ? 1 : 2;
        }
    }
    return count;
}
