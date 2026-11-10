/*
 * A283807  Smallest prime p such that A005117(k+1) - A005117(k-1) = n, where
 *          p = A005117(k) for some k.
 * https://oeis.org/A283807
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   search -- a(n) = smallest prime p = A005117(k) with A005117(k+1)
 *            - A005117(k-1) = n. Small p are found by direct scan; larger p
 *            by a segmented bitset sieve of non-squarefree numbers (squares
 *            of 2,3,5,7 via a periodic pattern mod 44100, larger prime
 *            squares marked), locating maximal runs of >= 5 c...
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 14, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 15 (2026-11-08)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

#define A283807_M 64
#define A283807_SB ((uint64_t)1 << 22)
#define A283807_SBW (A283807_SB / 64)
#define A283807_BL (A283807_SB + 1024)
#define A283807_BLW (A283807_BL / 64)
#define A283807_PER 44100
#define A283807_PLIM ((uint32_t)1 << 22)
#define A283807_BRUTE 1000000

static uint64_t A283807_found[A283807_M];
static int A283807_init = 0;
static int A283807_started = 0;
static uint64_t A283807_B0 = 0;
static uint64_t *A283807_pat = 0;
static uint64_t *A283807_buf = 0;
static uint32_t *A283807_pr = 0;
static uint64_t *A283807_nx = 0;
static size_t A283807_npr = 0, A283807_nact = 0;

static bool A283807_sqf_direct(uint64_t k) {
    if (k % 4 == 0) return false;
    for (uint64_t i = 3; i * i <= k; i += 2)
        if (k % (i * i) == 0) return false;
    return true;
}

static void A283807_setup(void) {
    /* direct scan for small p, all gaps */
    uint32_t lim = A283807_BRUTE + 512;
    uint8_t *sf = (uint8_t *)malloc(lim);
    uint8_t *cp = (uint8_t *)malloc(lim);
    for (uint32_t i = 0; i < lim; i++) { sf[i] = 1; cp[i] = 0; }
    for (uint32_t q = 2; (uint64_t)q * q < lim; q++)
        for (uint32_t m = q * q; m < lim; m += q * q) sf[m] = 0;
    for (uint32_t q = 2; (uint64_t)q * q < lim; q++)
        if (!cp[q])
            for (uint32_t m = q * q; m < lim; m += q) cp[m] = 1;
    for (uint32_t p = 2; p < A283807_BRUTE; p++) {
        if (cp[p]) continue;
        uint32_t lo = p - 1;
        while (!sf[lo]) lo--;
        uint32_t up = p + 1;
        while (!sf[up]) up++;
        uint32_t g = up - lo;
        if (g < A283807_M && !A283807_found[g]) A283807_found[g] = p;
    }
    free(sf);
    free(cp);

    /* periodic small-square pattern mod 44100, extended by 64 bits */
    size_t pw = (A283807_PER + 64 + 63) / 64 + 2;
    A283807_pat = (uint64_t *)calloc(pw, sizeof(uint64_t));
    for (uint32_t i = 0; i < A283807_PER + 64; i++) {
        uint32_t r = i % A283807_PER;
        if (r % 4 == 0 || r % 9 == 0 || r % 25 == 0 || r % 49 == 0)
            A283807_pat[i >> 6] |= (uint64_t)1 << (i & 63);
    }

    /* primes >= 11 below PLIM */
    uint8_t *c = (uint8_t *)calloc(A283807_PLIM, 1);
    for (uint32_t i = 2; (uint64_t)i * i < A283807_PLIM; i++)
        if (!c[i])
            for (uint32_t m = i * i; m < A283807_PLIM; m += i) c[m] = 1;
    size_t cnt = 0;
    for (uint32_t i = 11; i < A283807_PLIM; i++) if (!c[i]) cnt++;
    A283807_pr = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    A283807_nx = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    for (uint32_t i = 11; i < A283807_PLIM; i++) if (!c[i]) A283807_pr[A283807_npr++] = i;
    free(c);

    A283807_buf = (uint64_t *)calloc(A283807_BLW + 2, sizeof(uint64_t));
    A283807_init = 1;
}

static inline bool A283807_nsf(uint64_t x) {
    if (x >= A283807_B0 && x < A283807_B0 + A283807_BL) {
        uint64_t r = x - A283807_B0;
        return (A283807_buf[r >> 6] >> (r & 63)) & 1;
    }
    return !A283807_sqf_direct(x);
}

static void A283807_check(uint64_t p, uint64_t g) {
    if (g >= 11 && g < A283807_M && !A283807_found[g] && is_prime_u64(p))
        A283807_found[g] = p;
}

static void A283807_process(uint64_t s) {
    uint64_t e = s;
    while (A283807_nsf(e + 1)) e++;
    uint64_t lo = s - 2;
    while (A283807_nsf(lo)) lo--;
    A283807_check(s - 1, e + 1 - lo);
    uint64_t up = e + 2;
    while (A283807_nsf(up)) up++;
    A283807_check(e + 1, up - (s - 1));
}

static void A283807_segment(void) {
    uint64_t *buf = A283807_buf;
    size_t fsw;
    if (!A283807_started) {
        A283807_B0 = 0;
        fsw = 0;
        A283807_started = 1;
    } else {
        A283807_B0 += A283807_SB;
        memmove(buf, buf + A283807_SBW, 16 * sizeof(uint64_t));
        fsw = 16;
    }
    uint64_t B0 = A283807_B0;
    uint64_t bufend = B0 + A283807_BL;
    uint64_t fs = B0 + 64 * (uint64_t)fsw;
    uint64_t o = fs % A283807_PER;
    for (size_t w = fsw; w < A283807_BLW; w++) {
        uint64_t i = o >> 6, sh = o & 63;
        uint64_t v = A283807_pat[i] >> sh;
        if (sh) v |= A283807_pat[i + 1] << (64 - sh);
        buf[w] = v;
        o += 64;
        if (o >= A283807_PER) o -= A283807_PER;
    }
    while (A283807_nact < A283807_npr &&
           (uint64_t)A283807_pr[A283807_nact] * A283807_pr[A283807_nact] < bufend) {
        A283807_nx[A283807_nact] = (uint64_t)A283807_pr[A283807_nact] * A283807_pr[A283807_nact];
        A283807_nact++;
    }
    for (size_t k = 0; k < A283807_nact; k++) {
        uint64_t q2 = (uint64_t)A283807_pr[k] * A283807_pr[k];
        uint64_t m = A283807_nx[k];
        while (m < bufend) {
            uint64_t r = m - B0;
            buf[r >> 6] |= (uint64_t)1 << (r & 63);
            m += q2;
        }
        A283807_nx[k] = m;
    }
    for (size_t w = 8; w < 8 + A283807_SBW; w++) {
        uint64_t s0 = buf[w], s1 = buf[w + 1], s2 = buf[w + 2], sp = buf[w - 1];
        uint64_t a0 = s0 & ((s0 >> 1) | (s1 << 63));
        uint64_t a1 = s1 & ((s1 >> 1) | (s2 << 63));
        uint64_t t4 = a0 & ((a0 >> 2) | (a1 << 62));
        uint64_t t5 = t4 & ((s0 >> 4) | (s1 << 60));
        uint64_t cand = t5 & ~((s0 << 1) | (sp >> 63));
        while (cand) {
            int b = __builtin_ctzll(cand);
            cand &= cand - 1;
            A283807_process(B0 + 64 * (uint64_t)w + (uint64_t)b);
        }
    }
}

seq_t a(idx_t n) {
    if (n < 2 || n >= A283807_M) return 0;
    if (!A283807_init) A283807_setup();
    while (!A283807_found[n]) A283807_segment();
    return (seq_t)A283807_found[n];
}
