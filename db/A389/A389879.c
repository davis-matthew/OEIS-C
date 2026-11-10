/*
 * A389879  A005117(k) - 1 where k is the least k such that A389412(k) = n.
 * https://oeis.org/A389879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m >= 1 such that m + A005117(i) is squarefree for 1
 *            <= i <= n and m + A005117(n+1) is not. Search over m with a
 *            wheel of residues mod 2^2*3^2*5^2*7^2(*11^2) allowed for the
 *            target n, then filter with tables of small primes p <= 47, then
 *            do a full squarefree test
 * Range:     u8 n <= 5, u16 n <= 30, u32 n <= 109, u64 n <= 109,
 *            u128 n <= 109, big any n
 * Validated: standard, pass, 64 terms through n = 105 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static uint64_t *A389879_sq = NULL;
static size_t A389879_sq_cnt = 0, A389879_sq_cap = 0;
static uint64_t A389879_sq_last = 0;

static bool A389879_sqf_small(uint64_t k) {
    if (k % 4 == 0) return false;
    for (uint64_t i = 3; i * i <= k; i += 2)
        if (k % (i * i) == 0) return false;
    return true;
}

static void A389879_ensure_sq(size_t need) {
    while (A389879_sq_cnt < need) {
        do { A389879_sq_last++; } while (!A389879_sqf_small(A389879_sq_last));
        if (A389879_sq_cnt >= A389879_sq_cap) {
            A389879_sq_cap = A389879_sq_cap ? A389879_sq_cap * 2 : 256;
            A389879_sq = (uint64_t *)realloc(A389879_sq, A389879_sq_cap * sizeof(uint64_t));
        }
        A389879_sq[A389879_sq_cnt++] = A389879_sq_last;
    }
}

static uint32_t *A389879_pr = NULL;
static size_t A389879_npr = 0;

static void A389879_init_primes(void) {
    if (A389879_pr) return;
    const uint32_t LIM = 100000;
    unsigned char *c = (unsigned char *)calloc(LIM + 1, 1);
    A389879_pr = (uint32_t *)malloc(sizeof(uint32_t) * 10000);
    for (uint32_t i = 2; i <= LIM; i++) {
        if (c[i]) continue;
        A389879_pr[A389879_npr++] = i;
        for (uint64_t j = (uint64_t)i * i; j <= LIM; j += i) c[j] = 1;
    }
    free(c);
}

static bool A389879_is_square(uint64_t x) {
    if (x < 2) return true;
    uint64_t r = x, y = (r + 1) / 2;
    while (y < r) { r = y; y = (r + x / r) / 2; }
    return r * r == x;
}

static bool A389879_sqf(uint64_t x) {
    size_t k = 0;
    uint64_t p = 2;
    for (;;) {
        if (k < A389879_npr) p = A389879_pr[k++];
        else p += 2;
        if (p * p * p > x) break;
        if (x % p == 0) {
            x /= p;
            if (x % p == 0) return false;
        }
    }
    return !A389879_is_square(x);
}

static uint64_t *A389879_res = NULL;
static size_t A389879_res_cap = 0;

static void A389879_grow(size_t need) {
    if (need <= A389879_res_cap) return;
    size_t nc = A389879_res_cap ? A389879_res_cap : 128;
    while (nc < need) nc *= 2;
    uint64_t *t = (uint64_t *)calloc(nc, sizeof(uint64_t));
    for (size_t i = 0; i < A389879_res_cap; i++) t[i] = A389879_res[i];
    free(A389879_res);
    A389879_res = t;
    A389879_res_cap = nc;
}

#define A389879_NTP 15
static const uint32_t A389879_tp[A389879_NTP] =
    {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47};

seq_t a(idx_t n) {
    A389879_init_primes();
    A389879_grow((size_t)n + 1);
    if (A389879_res[n]) return (seq_t)A389879_res[n];

    size_t L = (size_t)n * 2 + 64;
    A389879_ensure_sq(L + 1);

    uint32_t *fi[A389879_NTP];
    uint64_t q[A389879_NTP];
    for (int t = 0; t < A389879_NTP; t++) {
        q[t] = (uint64_t)A389879_tp[t] * A389879_tp[t];
        fi[t] = (uint32_t *)malloc(sizeof(uint32_t) * q[t]);
        for (uint64_t r = 0; r < q[t]; r++) fi[t][r] = (uint32_t)(L + 1);
        for (size_t i = L; i >= 1; i--)
            fi[t][A389879_sq[i - 1] % q[t]] = (uint32_t)i;
    }

    /* wheel mod 44100 */
    const uint64_t W1 = 44100;
    uint64_t *l1 = (uint64_t *)malloc(sizeof(uint64_t) * W1);
    size_t n1 = 0;
    for (uint64_t r = 0; r < W1; r++) {
        bool ok = true;
        for (int t = 0; t < 4 && ok; t++)
            if (fi[t][(q[t] - r % q[t]) % q[t]] <= n) ok = false;
        if (ok) l1[n1++] = r;
    }
    uint64_t W = W1;
    uint64_t *wl = l1;
    size_t nw = n1;
    if (n1 * 121 <= 2000000) {
        uint64_t *l2 = (uint64_t *)malloc(sizeof(uint64_t) * (n1 * 121 + 1));
        size_t n2 = 0;
        for (uint64_t k = 0; k < 121; k++)
            for (size_t j = 0; j < n1; j++) {
                uint64_t r = l1[j] + W1 * k;
                if (fi[4][(121 - r % 121) % 121] > n) l2[n2++] = r;
            }
        free(l1);
        wl = l2; nw = n2; W = W1 * 121;
    }

    for (uint64_t base = 0; !A389879_res[n]; base += W) {
        for (size_t w = 0; w < nw; w++) {
            uint64_t m = base + wl[w];
            if (m == 0) continue;
            size_t F = L + 1;
            bool ok = true;
            for (int t = 0; t < A389879_NTP; t++) {
                uint32_t f = fi[t][(q[t] - m % q[t]) % q[t]];
                if (f <= n) { ok = false; break; }
                if (f < F) F = f;
            }
            if (!ok) continue;
            size_t j = 0;
            for (size_t i = 1;; i++) {
                if (i == F && F <= L) break;
                A389879_ensure_sq(i);
                if (!A389879_sqf(m + A389879_sq[i - 1])) break;
                j = i;
            }
            if (j >= n) {
                A389879_grow(j + 1);
                if (A389879_res[j] == 0) A389879_res[j] = m;
                if (A389879_res[n]) break;
            }
        }
    }

    free(wl);
    for (int t = 0; t < A389879_NTP; t++) free(fi[t]);
    return (seq_t)A389879_res[n];
}
