/*
 * A345984  Partial sums of A011772.
 * https://oeis.org/A345984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A011772(k), where A011772(k) is the smallest
 *            m>=1 with k | m(m+1)/2; each term is found by splitting 2k into
 *            coprime parts A|m, B|m+1 and solving by CRT
 * Range:     u8 n <= 24, u16 n <= 520, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 77 terms through n = 9367 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int64_t A345984_inv(int64_t a, int64_t b)
{
    int64_t g = b, x = 0, y = 1, aa = a % b;
    int64_t r0 = b, r1 = aa, s0 = 0, s1 = 1;
    (void)g; (void)x; (void)y;
    while (r1 != 0) {
        int64_t q = r0 / r1, t;
        t = r0 - q * r1; r0 = r1; r1 = t;
        t = s0 - q * s1; s0 = s1; s1 = t;
    }
    s0 %= b;
    if (s0 < 0) s0 += b;
    return s0;
}

static uint64_t A011772_a(uint64_t k)
{
    uint64_t N = 2 * k;
    uint64_t q[20];
    int c = 0;
    uint64_t x = N;
    for (uint64_t p = 2; p * p <= x; p++) {
        if (x % p == 0) {
            uint64_t pp = 1;
            while (x % p == 0) { x /= p; pp *= p; }
            q[c++] = pp;
        }
    }
    if (x > 1) q[c++] = x;
    uint64_t best = N;
    for (unsigned mask = 0; mask < (1u << c); mask++) {
        uint64_t A = 1;
        for (int i = 0; i < c; i++)
            if (mask & (1u << i)) A *= q[i];
        uint64_t B = N / A;
        if (B == 1) continue; /* m = N, already best's initial value */
        int64_t inv = A345984_inv((int64_t)(A % B), (int64_t)B);
        uint64_t t = ((uint64_t)B - (uint64_t)inv) % B;
        uint64_t m = A * t;
        if (m == 0) continue;
        if (m < best) best = m;
    }
    return best;
}

seq_t a(idx_t n)
{
    seq_t s = 0;
    for (idx_t k = 1; k <= n; k++)
        s += (seq_t)A011772_a((uint64_t)k);
    return s;
}
