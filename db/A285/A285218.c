/*
 * A285218  Indices of primes in A001156.
 * https://oeis.org/A285218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) is the n-th k >= 1 such that A001156(k) is prime.
 *            A001156(k) is computed exactly with multi-limb integers using
 *            the coin-change DP over squares (G.f. Product_{m>=1}
 *            1/(1-x^(m^2))). Each value is trial-divided by the primes below
 *            200, then given a Miller-Rabin test to bases 2..37. Tha...
 * Range:     u8 n <= 40, u16 n <= 938, u32 n <= 2119, u64 n <= 2119,
 *            u128 n <= 2119, big any n
 * Validated: standard, pass, 73 terms through n = 1857 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define A285218_MAXW 128

static uint64_t *A285218_found = NULL;
static size_t A285218_cnt = 0;
static size_t A285218_cap = 0;
static uint64_t A285218_scanned = 0;
static uint64_t A285218_limit = 0;

static int A285218_cmp(const uint64_t *x, const uint64_t *y, int W) {
    for (int i = W - 1; i >= 0; i--) {
        if (x[i] != y[i]) return x[i] < y[i] ? -1 : 1;
    }
    return 0;
}

static void A285218_sub(uint64_t *x, const uint64_t *y, int W) {
    uint64_t borrow = 0;
    for (int i = 0; i < W; i++) {
        uint64_t yi = y[i];
        uint64_t d = x[i] - yi - borrow;
        borrow = (x[i] < yi || (x[i] == yi && borrow)) ? 1 : 0;
        x[i] = d;
    }
}

static uint64_t A285218_mod_small(const uint64_t *x, int W, uint64_t p) {
    uint64_t r = 0;
    for (int i = W - 1; i >= 0; i--) {
        r = ((r << 32) | (x[i] >> 32)) % p;
        r = ((r << 32) | (x[i] & 0xFFFFFFFFu)) % p;
    }
    return r;
}

/* out = a*b*R^-1 mod N (CIOS Montgomery), inputs < N, output < N */
static void A285218_mm(const uint64_t *x, const uint64_t *y, const uint64_t *N,
                       uint64_t ninv, int W, uint64_t *out) {
    uint64_t t[A285218_MAXW + 2];
    for (int i = 0; i < W + 2; i++) t[i] = 0;
    for (int i = 0; i < W; i++) {
        unsigned __int128 cs;
        uint64_t C = 0;
        for (int j = 0; j < W; j++) {
            cs = (unsigned __int128)t[j] + (unsigned __int128)x[j] * y[i] + C;
            t[j] = (uint64_t)cs;
            C = (uint64_t)(cs >> 64);
        }
        cs = (unsigned __int128)t[W] + C;
        t[W] = (uint64_t)cs;
        t[W + 1] = (uint64_t)(cs >> 64);
        uint64_t m = t[0] * ninv;
        cs = (unsigned __int128)t[0] + (unsigned __int128)m * N[0];
        C = (uint64_t)(cs >> 64);
        for (int j = 1; j < W; j++) {
            cs = (unsigned __int128)t[j] + (unsigned __int128)m * N[j] + C;
            t[j - 1] = (uint64_t)cs;
            C = (uint64_t)(cs >> 64);
        }
        cs = (unsigned __int128)t[W] + C;
        t[W - 1] = (uint64_t)cs;
        t[W] = t[W + 1] + (uint64_t)(cs >> 64);
    }
    if (t[W] || A285218_cmp(t, N, W) >= 0) A285218_sub(t, N, W);
    for (int i = 0; i < W; i++) out[i] = t[i];
}

/* out = v * R mod N, by repeated doubling */
static void A285218_to_mont(uint64_t v, const uint64_t *N, int W, uint64_t *out) {
    for (int i = 0; i < W; i++) out[i] = 0;
    out[0] = v;
    for (int k = 0; k < 64 * W; k++) {
        uint64_t c = out[W - 1] >> 63;
        for (int i = W - 1; i > 0; i--) out[i] = (out[i] << 1) | (out[i - 1] >> 63);
        out[0] <<= 1;
        if (c || A285218_cmp(out, N, W) >= 0) A285218_sub(out, N, W);
    }
}

static bool A285218_isprime(const uint64_t *N, int W) {
    static const uint64_t sp[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,
        59,61,67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,
        157,163,167,173,179,181,191,193,197,199};
    static const uint64_t bases[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    while (W > 1 && N[W - 1] == 0) W--;
    if (W == 1 && N[0] < 2) return false;
    for (size_t i = 0; i < sizeof(sp) / sizeof(sp[0]); i++) {
        if (A285218_mod_small(N, W, sp[i]) == 0) return W == 1 && N[0] == sp[i];
    }
    if (W == 1 && N[0] < 44521) return true;

    uint64_t ninv, inv = N[0];
    for (int i = 0; i < 6; i++) inv *= 2 - N[0] * inv;
    ninv = (uint64_t)0 - inv;

    uint64_t d[A285218_MAXW], one[A285218_MAXW], m1[A285218_MAXW];
    uint64_t bm[A285218_MAXW], x[A285218_MAXW];
    for (int i = 0; i < W; i++) d[i] = N[i];
    d[0] -= 1; /* N odd */
    int s = 0;
    while (!(d[0] & 1)) {
        for (int i = 0; i < W - 1; i++) d[i] = (d[i] >> 1) | (d[i + 1] << 63);
        d[W - 1] >>= 1;
        s++;
    }
    int top = 64 * W - 1;
    while (top >= 0 && !((d[top / 64] >> (top % 64)) & 1)) top--;

    A285218_to_mont(1, N, W, one);
    for (int i = 0; i < W; i++) m1[i] = N[i];
    A285218_sub(m1, one, W);

    for (size_t b = 0; b < sizeof(bases) / sizeof(bases[0]); b++) {
        A285218_to_mont(bases[b], N, W, bm);
        for (int i = 0; i < W; i++) x[i] = one[i];
        for (int bit = top; bit >= 0; bit--) {
            A285218_mm(x, x, N, ninv, W, x);
            if ((d[bit / 64] >> (bit % 64)) & 1) A285218_mm(x, bm, N, ninv, W, x);
        }
        if (A285218_cmp(x, one, W) == 0 || A285218_cmp(x, m1, W) == 0) continue;
        bool ok = false;
        for (int r = 1; r < s; r++) {
            A285218_mm(x, x, N, ninv, W, x);
            if (A285218_cmp(x, m1, W) == 0) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

static void A285218_extend(uint64_t L) {
    int W = 1;
    uint64_t *dp;
    for (;;) {
        dp = (uint64_t *)calloc((size_t)(L + 1) * (size_t)W, sizeof(uint64_t));
        dp[0] = 1;
        bool overflow = false;
        for (uint64_t m = 1; m * m <= L && !overflow; m++) {
            uint64_t sq = m * m;
            for (uint64_t i = sq; i <= L; i++) {
                uint64_t *dst = dp + i * W;
                const uint64_t *src = dp + (i - sq) * W;
                uint64_t c = 0;
                for (int j = 0; j < W; j++) {
                    uint64_t s1 = dst[j] + src[j];
                    uint64_t c1 = s1 < dst[j];
                    uint64_t s2 = s1 + c;
                    uint64_t c2 = s2 < s1;
                    dst[j] = s2;
                    c = c1 | c2;
                }
                if (c) { overflow = true; break; }
            }
        }
        if (!overflow) break;
        free(dp);
        W++;
    }
    for (uint64_t k = A285218_scanned + 1; k <= L; k++) {
        if (A285218_isprime(dp + k * W, W)) {
            if (A285218_cnt == A285218_cap) {
                A285218_cap = A285218_cap ? A285218_cap * 2 : 256;
                A285218_found = (uint64_t *)realloc(A285218_found, A285218_cap * sizeof(uint64_t));
            }
            A285218_found[A285218_cnt++] = k;
        }
    }
    A285218_scanned = L;
    A285218_limit = L;
    free(dp);
}

seq_t a(idx_t n) {
    while (A285218_cnt < (size_t)n) {
        uint64_t L = A285218_limit ? A285218_limit * 2 : 512;
        A285218_extend(L);
    }
    return (seq_t)A285218_found[n - 1];
}
