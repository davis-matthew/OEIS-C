/*
 * A292868  Records in A243823.
 * https://oeis.org/A292868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th record value of A243823(k) = k + 1 - phi(k) - #{m <=
 *            k : rad(m) | k}, found by a segmented sieve for phi(k) and the
 *            small prime factors of k, scanning k = 1, 2, 3,
 * Range:     u8 n <= 79, u16 n <= 541, u32 n <= 1500, u64 n <= 1500,
 *            u128 n <= 1500, big any n
 * Validated: standard, pass, 79 terms through n = 1453 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#define R292868_BS 65536
#define R292868_MAXPF 16

static uint32_t *R292868_pr = NULL;
static size_t R292868_prcnt = 0;
static uint64_t R292868_prlim = 0;

static uint64_t *R292868_rec = NULL;
static size_t R292868_reccnt = 0, R292868_reccap = 0;
static uint64_t R292868_next = 1;      /* next k to scan */
static long long R292868_best = -1;

static uint64_t R292868_phiB[R292868_BS];
static uint32_t R292868_pfB[R292868_BS][R292868_MAXPF];
static unsigned char R292868_pcB[R292868_BS];

static uint64_t R292868_isqrt(uint64_t x)
{
    uint64_t r = 0;
    uint64_t bit = (uint64_t)1 << 62;
    while (bit > x) bit >>= 2;
    while (bit) {
        if (x >= r + bit) { x -= r + bit; r = (r >> 1) + bit; }
        else r >>= 1;
        bit >>= 2;
    }
    return r;
}

static void R292868_ensure_primes(uint64_t need)
{
    if (R292868_prlim >= need) return;
    uint64_t L = need * 2 + 100;
    unsigned char *mark = (unsigned char *)malloc((size_t)L + 1);
    for (uint64_t i = 0; i <= L; ++i) mark[i] = 1;
    mark[0] = mark[1] = 0;
    for (uint64_t i = 2; i * i <= L; ++i)
        if (mark[i])
            for (uint64_t j = i * i; j <= L; j += i) mark[j] = 0;
    size_t cnt = 0;
    for (uint64_t i = 2; i <= L; ++i) if (mark[i]) ++cnt;
    free(R292868_pr);
    R292868_pr = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    R292868_prcnt = 0;
    for (uint64_t i = 2; i <= L; ++i) if (mark[i]) R292868_pr[R292868_prcnt++] = (uint32_t)i;
    free(mark);
    R292868_prlim = L;
}

/* count m <= lim with all prime factors among p[i..c-1], times cur */
static uint64_t R292868_regcount(const uint64_t *p, int c, int i,
                                 uint64_t cur, uint64_t lim)
{
    if (i == c) return 1;
    uint64_t total = 0;
    uint64_t v = cur;
    for (;;) {
        total += R292868_regcount(p, c, i + 1, v, lim);
        if (v > lim / p[i]) break;
        v *= p[i];
    }
    return total;
}

static void R292868_push(uint64_t v)
{
    if (R292868_reccnt + 1 > R292868_reccap) {
        size_t nc = R292868_reccap ? R292868_reccap * 2 : 256;
        R292868_rec = (uint64_t *)realloc(R292868_rec, nc * sizeof(uint64_t));
        R292868_reccap = nc;
    }
    R292868_rec[R292868_reccnt++] = v;
}

static void R292868_block(void)
{
    uint64_t lo = R292868_next, hi = lo + R292868_BS;
    uint64_t s = R292868_isqrt(hi - 1);
    R292868_ensure_primes(s);
    for (size_t i = 0; i < R292868_BS; ++i) {
        R292868_phiB[i] = lo + i;
        R292868_pcB[i] = 0;
    }
    for (size_t t = 0; t < R292868_prcnt; ++t) {
        uint64_t p = R292868_pr[t];
        if (p > s) break;
        uint64_t start = ((lo + p - 1) / p) * p;
        for (uint64_t j = start; j < hi; j += p) {
            size_t i = (size_t)(j - lo);
            R292868_phiB[i] -= R292868_phiB[i] / p;
            R292868_pfB[i][R292868_pcB[i]++] = (uint32_t)p;
        }
    }
    uint64_t sq = R292868_isqrt(lo);
    if (sq < 1) sq = 1;
    for (size_t i = 0; i < R292868_BS; ++i) {
        uint64_t k = lo + i;
        uint64_t ph = R292868_phiB[i];
        /* upper bound on k+1-phi(k), allowing for one prime factor > s */
        uint64_t ub = k + 1 - ph + ph / sq + 1;
        if ((long long)ub <= R292868_best) continue;
        uint64_t pr[R292868_MAXPF + 1];
        int c = 0;
        uint64_t rem = k;
        for (int t = 0; t < R292868_pcB[i]; ++t) {
            uint64_t p = R292868_pfB[i][t];
            pr[c++] = p;
            while (rem % p == 0) rem /= p;
        }
        if (rem > 1) {
            pr[c++] = rem;
            ph = ph / rem * (rem - 1);
        }
        uint64_t reg = R292868_regcount(pr, c, 0, 1, k);
        long long v = (long long)(k + 1) - (long long)ph - (long long)reg;
        if (v > R292868_best) {
            R292868_best = v;
            R292868_push((uint64_t)v);
        }
    }
    R292868_next = hi;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    if (m < 1) m = 1;
    while (R292868_reccnt < m) R292868_block();
    return (seq_t)R292868_rec[m - 1];
}
