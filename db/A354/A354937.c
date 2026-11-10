/*
 * A354937  Row 7 of A354940: Numbers k for which A345992(k) = 7, divided by
 *          7.
 * https://oeis.org/A354937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number t (in increasing order) with A345992(7*t) =
 *            7; t is searched among 7j+1, 7j+4, 7j+5
 * Range:     u8 n <= 52, u16 n <= 7075, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9793 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static uint64_t A354937_gcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static uint64_t A354937_modinv(uint64_t a, uint64_t mod) {
    int64_t x0 = 1, x1 = 0, aa = (int64_t)a, bb = (int64_t)mod;
    while (bb != 0) {
        int64_t q = aa / bb, t = aa % bb;
        aa = bb; bb = t;
        t = x0 - q * x1; x0 = x1; x1 = t;
    }
    x0 %= (int64_t)mod;
    if (x0 < 0) x0 += (int64_t)mod;
    return (uint64_t)x0;
}

/* A345992(k): gcd(k, m) with m smallest positive solution of k | m(m+1) */
static uint64_t A354937_a345992(uint64_t k) {
    uint64_t pm[20];
    size_t c = 0;
    uint64_t x = k;
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            uint64_t pe = 1;
            while (x % p == 0) { x /= p; pe *= p; }
            pm[c++] = pe;
        }
    }
    if (x > 1) pm[c++] = x;
    uint64_t best = k;
    size_t total = (size_t)1 << c;
    for (size_t mask = 0; mask < total; ++mask) {
        uint64_t r = 0, mod = 1;
        for (size_t i = 0; i < c; ++i) {
            uint64_t m = pm[i];
            uint64_t a = (mask & ((size_t)1 << i)) ? (m - 1) : 0;
            uint64_t inv = A354937_modinv(mod % m, m);
            uint64_t diff = (a + m - (r % m)) % m;
            uint64_t kk = (diff * inv) % m;
            r = r + mod * kk;
            mod *= m;
        }
        uint64_t cand = (r == 0) ? k : r;
        if (cand < best) best = cand;
    }
    return A354937_gcd(k, best);
}

seq_t a(idx_t n) {
    static const uint64_t offs[3] = {1, 4, 5};
    uint64_t count = 0;
    for (uint64_t j = 0;; ++j) {
        for (int o = 0; o < 3; ++o) {
            uint64_t t = 7 * j + offs[o];
            if (A354937_a345992(7 * t) == 7) {
                if (++count == (uint64_t)n) return (seq_t)t;
            }
        }
    }
}
