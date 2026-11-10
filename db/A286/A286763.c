/*
 * A286763  Numbers that appear in A195441 at least once for two consecutive
 *          indices.
 * https://oeis.org/A286763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. Compute A195441(k) for k = 0..K, where A195441(k) =
 *            product of * primes p <= (k+2)/(2+(k mod 2)) such that the
 *            base-p digit sum of k+1 is >= p. * Collect the distinct values v
 *            with A195441(k) = A195441(k+1) = v, sorted increasingly. * K
 *            doubles until the n-th smallest such value is below...
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/mem.h"

static uint32_t *S_pr = NULL;
static size_t S_pc = 0;
static uint64_t S_plim = 0;

static double *S_L = NULL;      /* sum of log p for A195441(k) */
static uint64_t *S_P = NULL;    /* A195441(k) mod 2^64 (fingerprint) */
static size_t S_done = 0, S_cap = 0;

static void S_sieve(uint64_t lim)
{
    if (lim <= S_plim) return;
    uint64_t L = S_plim ? S_plim : 1024;
    while (L < lim) L *= 2;
    uint8_t *c = (uint8_t *)calloc(L + 1, 1);
    if (!c) abort();
    free(S_pr);
    S_pr = (uint32_t *)malloc(sizeof(uint32_t) * (L / 2 + 2));
    if (!S_pr) abort();
    S_pc = 0;
    for (uint64_t i = 2; i <= L; i++) {
        if (!c[i]) {
            S_pr[S_pc++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= L; j += i) c[j] = 1;
        }
    }
    free(c);
    S_plim = L;
}

static bool S_divides(uint64_t N, uint64_t p)
{
    uint64_t t = N, s = 0;
    while (t) { s += t % p; t /= p; }
    return s >= p;
}

static void S_compute_upto(uint64_t K)
{
    if (K + 1 <= S_done) return;
    S_sieve(K / 2 + 4);
    if (S_cap < K + 1) {
        size_t nc = (size_t)K + 1;
        S_L = (double *)realloc(S_L, sizeof(double) * nc);
        S_P = (uint64_t *)realloc(S_P, sizeof(uint64_t) * nc);
        if (!S_L || !S_P) abort();
        S_cap = nc;
    }
    for (uint64_t k = S_done; k <= K; k++) {
        uint64_t bound = (k + 2) / (2 + (k & 1ULL));
        uint64_t N = k + 1;
        double ls = 0.0;
        uint64_t pm = 1;
        for (size_t i = 0; i < S_pc && S_pr[i] <= bound; i++) {
            uint64_t p = S_pr[i];
            if (S_divides(N, p)) { ls += log((double)p); pm *= p; }
        }
        S_L[k] = ls;
        S_P[k] = pm;
    }
    S_done = (size_t)K + 1;
}

typedef struct { double l; uint64_t p; uint64_t k; } S_cand;

static int S_cmp(const void *x, const void *y)
{
    const S_cand *a1 = (const S_cand *)x, *b1 = (const S_cand *)y;
    if (a1->l < b1->l - 1e-9) return -1;
    if (a1->l > b1->l + 1e-9) return 1;
    if (a1->p < b1->p) return -1;
    if (a1->p > b1->p) return 1;
    return 0;
}

static seq_t S_value(uint64_t k)
{
    uint64_t bound = (k + 2) / (2 + (k & 1ULL));
    uint64_t N = k + 1;
    seq_t prod = 1;
    for (size_t i = 0; i < S_pc && S_pr[i] <= bound; i++) {
        uint64_t p = S_pr[i];
        if (S_divides(N, p)) prod *= (seq_t)p;
    }
    return prod;
}

seq_t a(idx_t n)
{
    uint64_t K = 4096;
    for (;;) {
        S_compute_upto(K);
        S_cand *c = (S_cand *)malloc(sizeof(S_cand) * (size_t)(K + 1));
        if (!c) abort();
        size_t cn = 0;
        for (uint64_t k = 0; k < K; k++) {
            if (S_P[k] == S_P[k + 1] && fabs(S_L[k] - S_L[k + 1]) < 1e-9) {
                c[cn].l = S_L[k]; c[cn].p = S_P[k]; c[cn].k = k; cn++;
            }
        }
        qsort(c, cn, sizeof(S_cand), S_cmp);
        size_t dn = 0;
        for (size_t i = 0; i < cn; i++) {
            if (dn > 0 && S_cmp(&c[dn - 1], &c[i]) == 0) continue;
            c[dn++] = c[i];
        }
        double B = 1e300;
        for (uint64_t k = K / 2; k <= K; k++)
            if (S_L[k] < B) B = S_L[k];
        size_t ok = 0;
        while (ok < dn && c[ok].l < B - 1e-6) ok++;
        if ((uint64_t)ok >= (uint64_t)n) {
            uint64_t kk = c[n - 1].k;
            free(c);
            return S_value(kk);
        }
        free(c);
        K *= 2;
    }
}
