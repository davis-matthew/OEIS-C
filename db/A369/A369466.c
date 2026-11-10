/*
 * A369466  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A369465(i) = A369465(j), for all i, j >= 1.
 * https://oeis.org/A369466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth transform of A369465, i.e. the rank of
 *            A369465(n) among distinct values in order of first appearance;
 *            computed by sieve-like incremental scan with hash table
 * Range:     u8 n <= 1224, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 118 terms through n = 9665 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static uint64_t A369465_d(uint64_t n)
{
    uint64_t m = n;
    while ((m & 1ULL) == 0ULL) m >>= 1;
    if (m <= 1ULL) return 0;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(m, primes, exps, &cnt);
    uint64_t r = 0;
    for (size_t i = 0; i < cnt; ++i)
        r += (m / primes[i]) * exps[i];
    return r;
}

static uint64_t *hk = NULL;
static uint32_t *hv = NULL;
static size_t hcap = 0, hcnt = 0;
static uint32_t *rk = NULL;
static size_t rkcap = 0;
static uint64_t done = 0;

static size_t hhash(uint64_t k, size_t cap)
{
    k ^= k >> 33;
    k *= 0xff51afd7ed558ccdULL;
    k ^= k >> 33;
    return (size_t)(k & (cap - 1));
}

static void hgrow(void)
{
    size_t ncap = hcap ? hcap * 2 : 1024;
    uint64_t *nk = (uint64_t *)calloc(ncap, sizeof(uint64_t));
    uint32_t *nv = (uint32_t *)calloc(ncap, sizeof(uint32_t));
    for (size_t i = 0; i < hcap; ++i) {
        if (hv[i]) {
            size_t p = hhash(hk[i], ncap);
            while (nv[p]) p = (p + 1) & (ncap - 1);
            nk[p] = hk[i];
            nv[p] = hv[i];
        }
    }
    free(hk);
    free(hv);
    hk = nk;
    hv = nv;
    hcap = ncap;
}

seq_t a(idx_t n)
{
    if (n > done) {
        if ((size_t)n + 1 > rkcap) {
            size_t nc = rkcap ? rkcap : 1024;
            while (nc < (size_t)n + 1) nc *= 2;
            rk = (uint32_t *)realloc(rk, nc * sizeof(uint32_t));
            rkcap = nc;
        }
        for (uint64_t i = done + 1; i <= (uint64_t)n; ++i) {
            if ((hcnt + 1) * 2 > hcap) hgrow();
            uint64_t v = A369465_d(i);
            size_t p = hhash(v, hcap);
            uint32_t r = 0;
            while (hv[p]) {
                if (hk[p] == v) { r = hv[p]; break; }
                p = (p + 1) & (hcap - 1);
            }
            if (!r) {
                hcnt++;
                hk[p] = v;
                hv[p] = (uint32_t)hcnt;
                r = (uint32_t)hcnt;
            }
            rk[i] = r;
        }
        done = (uint64_t)n;
    }
    return (seq_t)rk[n];
}
