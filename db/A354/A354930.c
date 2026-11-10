/*
 * A354930  Square array where the row n lists all nonnegative numbers k for
 *          which A345992(k) = n, read by falling antidiagonals.
 * https://oeis.org/A354930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(i, d+1-i), the k-th x>=1 with A345992(x) = i, read by
 *            antidiagonals (found by search)
 * Range:     u8 n <= 64, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static uint64_t A354930_gcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static void A354930_factor(uint64_t n, uint64_t *mods, size_t *cnt) {
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t pe = 1;
            while (n % p == 0) { n /= p; pe *= p; }
            mods[c++] = pe;
        }
    }
    if (n > 1) mods[c++] = n;
    *cnt = c;
}

static int64_t A354930_egcd(int64_t a, int64_t b, int64_t *x, int64_t *y) {
    int64_t x0 = 1, y0 = 0, x1 = 0, y1 = 1;
    while (b != 0) {
        int64_t q = a / b;
        int64_t t = a % b;
        a = b; b = t;
        t = x0 - q * x1; x0 = x1; x1 = t;
        t = y0 - q * y1; y0 = y1; y1 = t;
    }
    *x = x0; *y = y0;
    return a;
}

static uint64_t A354930_modinv(uint64_t a, uint64_t mod) {
    int64_t x, y;
    A354930_egcd((int64_t)a, (int64_t)mod, &x, &y);
    x %= (int64_t)mod;
    if (x < 0) x += (int64_t)mod;
    return (uint64_t)x;
}

static uint64_t A354930_crt(uint64_t r, uint64_t m, uint64_t a, uint64_t mp) {
    uint64_t inv = A354930_modinv(m % mp, mp);
    uint64_t diff = (a + mp - (r % mp)) % mp;
    uint64_t k = (diff * inv) % mp;
    return r + m * k;
}

static uint64_t A354930_A345992(uint64_t nn) {
    if (nn == 0) return 0;
    uint64_t mods[16];
    size_t k;
    A354930_factor(nn, mods, &k);
    uint64_t best = nn;
    size_t total = (size_t)1 << k;
    for (size_t mask = 0; mask < total; ++mask) {
        uint64_t r = 0, mod = 1;
        for (size_t i = 0; i < k; ++i) {
            uint64_t mi = mods[i];
            uint64_t ai = (mask & ((size_t)1 << i)) ? (mi - 1) : 0;
            r = A354930_crt(r, mod, ai, mi);
            mod *= mi;
        }
        uint64_t cand = (r == 0) ? nn : r;
        if (cand < best) best = cand;
    }
    return A354930_gcd(nn, best);
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    uint64_t d = 1;
    while (d * (d + 1) / 2 < N) d++;
    uint64_t i = N - d * (d - 1) / 2;   /* row */
    uint64_t col = d + 1 - i;           /* column */
    uint64_t cnt = 0, x = 0;
    while (cnt < col) {
        x++;
        if (A354930_A345992(x) == i) cnt++;
    }
    return (seq_t)x;
}
