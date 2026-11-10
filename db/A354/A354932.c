/*
 * A354932  a(n) = 1/n * {the least k for which A345992(k) = n}.
 * https://oeis.org/A354932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A354931(n)/n, where A354931(n) is the least k with
 *            A345992(k) = n; found by searching k = n*j for j = 1, 2,
 * Range:     u8 n <= 599, u16 n <= 3349, u32 n <= 3349, u64 n <= 3349,
 *            u128 n <= 3349, big any n
 * Validated: standard, pass, 110 terms through n = 3129 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static int64_t A354932_inv(int64_t a, int64_t mod) {
    int64_t b = mod, x0 = 1, x1 = 0;
    a %= mod;
    if (a < 0) a += mod;
    while (b != 0) {
        int64_t q = a / b, t = a % b;
        a = b; b = t;
        t = x0 - q * x1; x0 = x1; x1 = t;
    }
    x0 %= mod;
    if (x0 < 0) x0 += mod;
    return x0;
}

static uint64_t A354932_gcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

/* A345992(k) computed with 64-bit integers */
static uint64_t A354932_A345992(uint64_t k) {
    uint64_t q[20];
    size_t c = 0;
    uint64_t t = k;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            uint64_t pe = 1;
            while (t % p == 0) { t /= p; pe *= p; }
            q[c++] = pe;
        }
    }
    if (t > 1) q[c++] = t;
    uint64_t best = k;
    size_t total = (size_t)1 << c;
    for (size_t mask = 0; mask < total; ++mask) {
        uint64_t r = 0, M = 1;
        for (size_t i = 0; i < c; ++i) {
            uint64_t a = (mask >> i & 1) ? q[i] - 1 : 0;
            uint64_t diff = (a + q[i] - r % q[i]) % q[i];
            uint64_t inv = (uint64_t)A354932_inv((int64_t)(M % q[i]), (int64_t)q[i]);
            uint64_t tt = (diff % q[i]) * inv % q[i];
            r += M * tt;
            M *= q[i];
        }
        if (r == 0) r = k;
        if (r < best) best = r;
    }
    return A354932_gcd(k, best);
}

seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    if (nn == 0) return 0;
    for (uint64_t j = 1;; ++j) {
        uint64_t k = nn * j;
        if (A354932_A345992(k) == nn) return (seq_t)j;
    }
}
