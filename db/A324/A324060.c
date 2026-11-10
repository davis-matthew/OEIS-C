/*
 * A324060  Smallest positive index m such that A000009(m) is divisible by n.
 * https://oeis.org/A324060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m >= 1 with A000009(m) == 0 (mod n); search,
 *            computing A000009(m) mod n by a(m) = e(m) + Sum_{k>=1}
 *            (-1)^(k+1)*(a(m-k(3k-1)/2) + a(m-k(3k+1)/2)), a(0)=1, e(m) =
 *            (-1)^j if m = 2*j(3j-1)/2 or 2*j(3j+1)/2, else 0
 * Range:     u8 n <= 120, u16 n <= 7708, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 82 terms through n = 9948 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int A324060_e2(uint64_t m)
{
    uint64_t h, j;
    if (m & 1) return 0;
    h = m / 2;
    for (j = 0;; j++) {
        uint64_t g1 = (j == 0) ? 0 : j * (3 * j - 1) / 2;
        uint64_t g2 = j * (3 * j + 1) / 2;
        if (g1 > h) break;
        if (g1 == h || g2 == h) return (j & 1) ? -1 : 1;
    }
    return 0;
}

static uint64_t A324060_add(uint64_t x, uint64_t y, uint64_t mod)
{
    unsigned __int128 r = (unsigned __int128)x + y;
    if (r >= mod) r -= mod;
    return (uint64_t)r;
}

static uint64_t A324060_sub(uint64_t x, uint64_t y, uint64_t mod)
{
    unsigned __int128 r = (unsigned __int128)x + mod - y;
    if (r >= mod) r -= mod;
    return (uint64_t)r;
}

seq_t a(idx_t n)
{
    uint64_t mod = (uint64_t)n;
    size_t cap = 256;
    uint64_t *q;
    uint64_t m;
    if (mod == 0) return (seq_t)0;
    q = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!q) return (seq_t)0;
    q[0] = 1 % mod;
    for (m = 1;; m++) {
        uint64_t acc = 0, k;
        int e;
        if ((size_t)m >= cap) {
            uint64_t *t;
            cap <<= 1;
            t = (uint64_t *)realloc(q, cap * sizeof(uint64_t));
            if (!t) { free(q); return (seq_t)0; }
            q = t;
        }
        e = A324060_e2(m);
        if (e > 0) acc = A324060_add(acc, 1 % mod, mod);
        else if (e < 0) acc = A324060_sub(acc, 1 % mod, mod);
        for (k = 1;; k++) {
            uint64_t g1 = k * (3 * k - 1) / 2;
            uint64_t g2 = k * (3 * k + 1) / 2;
            uint64_t t = 0;
            if (g1 > m) break;
            t = q[m - g1];
            if (g2 <= m) t = A324060_add(t, q[m - g2], mod);
            if (k & 1) acc = A324060_add(acc, t, mod);
            else acc = A324060_sub(acc, t, mod);
        }
        q[m] = acc;
        if (acc == 0) {
            free(q);
            return (seq_t)m;
        }
    }
}
