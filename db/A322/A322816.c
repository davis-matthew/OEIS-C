/*
 * A322816  Lexicographically earliest sequence such that a(i) = a(j) => f(i)
 *          = f(j) for all i, j, where f(n) = A048675(n) for all other
 *          numbers, except f(n) = -1 if n is an odd prime.
 * https://oeis.org/A322816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rank of first occurrence of f(n) among f(1),f(2),...;
 *            f(n) = A048675(n), except f(n) = -1 for odd primes n (restricted
 *            growth / lexicographically earliest sequence, computed by a
 *            sieve with hashed exact big keys)
 * Range:     u8 n <= 572, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9703 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint32_t *A322816_res = 0;
static uint64_t A322816_N = 0;

static uint64_t A322816_mix(uint64_t x)
{
    x ^= x >> 33; x *= 0xff51afd7ed558ccdULL;
    x ^= x >> 33; x *= 0xc4ceb9fe1a85ec53ULL;
    x ^= x >> 33;
    return x;
}

static void A322816_build(uint64_t N)
{
    uint32_t *spf = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    uint32_t *pidx = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    uint32_t cnt = 0;
    for (uint64_t i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            cnt++;
            pidx[i] = cnt;
            for (uint64_t j = i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    size_t W = cnt / 32 + 4;
    uint32_t *buf = (uint32_t *)malloc(W * sizeof(uint32_t));
    uint64_t tsz = 1;
    while (tsz < 2 * N + 2) tsz <<= 1;
    uint64_t *H1 = (uint64_t *)calloc(tsz, sizeof(uint64_t));
    uint64_t *H2 = (uint64_t *)calloc(tsz, sizeof(uint64_t));
    uint32_t *HI = (uint32_t *)calloc(tsz, sizeof(uint32_t));
    uint32_t *res = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    uint32_t nid = 0;

    for (uint64_t m = 1; m <= N; m++) {
        uint64_t h1, h2;
        if (m > 2 && spf[m] == m) {
            h1 = 0x9e3779b97f4a7c15ULL;
            h2 = 0x1234567887654321ULL;
        } else {
            memset(buf, 0, W * sizeof(uint32_t));
            uint64_t r = m;
            while (r > 1) {
                uint32_t p = spf[r];
                uint32_t e = 0;
                while (r % p == 0) { r /= p; e++; }
                uint32_t b = pidx[p] - 1;
                size_t w = b / 32;
                uint64_t v = (uint64_t)e << (b % 32);
                while (v && w < W) {
                    v += buf[w];
                    buf[w] = (uint32_t)v;
                    v >>= 32;
                    w++;
                }
            }
            h1 = 0x243f6a8885a308d3ULL;
            h2 = 0x13198a2e03707344ULL;
            for (size_t i = 0; i < W; i++) {
                h1 = A322816_mix(h1 ^ (buf[i] + 0x9e3779b97f4a7c15ULL * (i + 1)));
                h2 = A322816_mix(h2 + buf[i] * 0xbf58476d1ce4e5b9ULL + i);
            }
            if (h1 == 0x9e3779b97f4a7c15ULL && h2 == 0x1234567887654321ULL) h2++;
        }
        uint64_t pos = h1 & (tsz - 1);
        while (HI[pos] && !(H1[pos] == h1 && H2[pos] == h2))
            pos = (pos + 1) & (tsz - 1);
        if (!HI[pos]) {
            HI[pos] = ++nid;
            H1[pos] = h1;
            H2[pos] = h2;
        }
        res[m] = HI[pos];
    }
    free(spf); free(pidx); free(buf); free(H1); free(H2); free(HI);
    if (A322816_res) free(A322816_res);
    A322816_res = res;
    A322816_N = N;
}

seq_t a(idx_t n)
{
    if ((uint64_t)n > A322816_N) {
        uint64_t N = (uint64_t)n;
        if (N < 2 * A322816_N) N = 2 * A322816_N;
        if (N < 1024) N = 1024;
        A322816_build(N);
    }
    return (seq_t)A322816_res[n];
}
