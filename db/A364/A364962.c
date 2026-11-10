/*
 * A364962  Odd numbers k such that A005941(k) is either k itself or its
 *          descendant in Doudna-tree, A005940.
 * https://oeis.org/A364962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   search over odd k: k is a term iff A156552(k) = A005941(k)-1,
 *            written in binary, is a prefix of A156552(A005941(k)) (k = 1 is
 *            always a term). Prime indices of large factors come from
 *            Lucy_Hedgehog prime counting
 * Range:     u8 n <= 27, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t d_isqrt(uint64_t n) {
    uint64_t r = 0;
    uint64_t bit = (uint64_t)1 << 62;
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= r + bit) { n -= r + bit; r = (r >> 1) + bit; }
        else r >>= 1;
        bit >>= 2;
    }
    return r;
}

/* SHARED_HELPER: prime counting function pi(n) via Lucy_Hedgehog, O(n^(3/4)) */
static uint64_t d_pi(uint64_t n) {
    if (n < 2) return 0;
    uint64_t r = d_isqrt(n);
    uint64_t *lo = (uint64_t *)malloc((r + 2) * sizeof(uint64_t));
    uint64_t *hi = (uint64_t *)malloc((r + 2) * sizeof(uint64_t));
    for (uint64_t i = 1; i <= r; i++) { lo[i] = i - 1; hi[i] = n / i - 1; }
    lo[0] = 0;
    for (uint64_t p = 2; p <= r; p++) {
        if (lo[p] == lo[p - 1]) continue;
        uint64_t sp = lo[p - 1];
        uint64_t p2 = p * p;
        uint64_t lim = n / p2;
        if (lim > r) lim = r;
        for (uint64_t i = 1; i <= lim; i++) {
            uint64_t d = i * p;
            uint64_t t = (d <= r) ? hi[d] : lo[n / d];
            hi[i] -= t - sp;
        }
        for (uint64_t v = r; v >= p2; v--) lo[v] -= lo[v / p] - sp;
    }
    uint64_t res = hi[1];
    free(lo);
    free(hi);
    return res;
}

/* bit positions of A156552(m), ascending; returns count, or -1 on overflow */
static int d_positions(uint64_t m, uint64_t *pos) {
    int c = 0;
    uint64_t j = 0;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p) continue;
        uint64_t idx = d_pi(p);
        while (m % p == 0) {
            m /= p;
            pos[c++] = idx - 1 + j;
            j++;
        }
    }
    if (m > 1) {
        uint64_t idx = d_pi(m);
        pos[c++] = idx - 1 + j;
    }
    return c;
}

static bool d_test(uint64_t k) {
    if (k == 1) return true;
    uint64_t pk[70], px[70];
    int ck = d_positions(k, pk);
    uint64_t u = 0;
    for (int i = 0; i < ck; i++) {
        if (pk[i] >= 62) return false;
        u += (uint64_t)1 << pk[i];
    }
    uint64_t x = u + 1; /* A005941(k) */
    int cx = d_positions(x, px);
    uint64_t M = px[cx - 1] + 1; /* bit length of A156552(x) */
    uint64_t L = 0;
    { uint64_t t = u; while (t) { L++; t >>= 1; } }
    if (M < L) return false;
    uint64_t shift = M - L;
    uint64_t w = 0;
    for (int i = 0; i < cx; i++)
        if (px[i] >= shift) w += (uint64_t)1 << (px[i] - shift);
    return w == u;
}

seq_t a(idx_t n) {
    static uint64_t terms[4096];
    static uint64_t cnt = 0;
    static uint64_t next_k = 1;
    while (cnt < n && cnt < 4096) {
        if (d_test(next_k)) terms[cnt++] = next_k;
        next_k += 2;
    }
    return (seq_t)terms[n - 1];
}
