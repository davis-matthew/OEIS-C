/*
 * A322815  Lexicographically earliest such sequence a that a(i) = a(j) =>
 *          A048675(i) = A048675(j) for all i, j.
 * https://oeis.org/A322815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth transform of A048675: a(n) = 1 +
 *            (number of distinct values of A048675(m) first seen before the
 *            first occurrence of A048675(n)), computed by sieve; A048675(m) =
 *            (1/2)*Sum_{p^e||m} e*2^PrimeIndex(p) is kept as a big bitset and
 *            compared via a 128-bit hash
 * Range:     u8 n <= 418, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 116 terms through n = 9687 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *rg_id = 0;
static size_t rg_limit = 0;

static void rg_add_at(uint64_t *w, size_t W, size_t i, uint64_t val)
{
    while (val && i < W) {
        uint64_t s = w[i] + val;
        val = (s < val) ? 1 : 0;
        w[i] = s;
        i++;
    }
}

static void rg_build(size_t L)
{
    uint32_t *spf = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    uint32_t *pidx = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    size_t i, j, npr = 0;
    for (i = 2; i <= L; i++) {
        if (spf[i] == 0) {
            npr++;
            pidx[i] = (uint32_t)npr;
            for (j = i; j <= L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    size_t W = npr / 64 + 3;
    size_t tsz = 1;
    while (tsz < 2 * L + 2) tsz <<= 1;
    uint64_t *th1 = (uint64_t *)calloc(tsz, sizeof(uint64_t));
    uint64_t *th2 = (uint64_t *)calloc(tsz, sizeof(uint64_t));
    uint32_t *tid = (uint32_t *)calloc(tsz, sizeof(uint32_t));
    uint64_t *words = (uint64_t *)malloc(W * sizeof(uint64_t));
    uint32_t *ids = (uint32_t *)calloc(L + 1, sizeof(uint32_t));
    uint32_t next_id = 0;
    size_t m;
    for (m = 1; m <= L; m++) {
        for (i = 0; i < W; i++) words[i] = 0;
        size_t x = m;
        while (x > 1) {
            uint32_t p = spf[x];
            uint64_t e = 0;
            while (x % p == 0) { x /= p; e++; }
            size_t k = pidx[p] - 1;
            size_t wi = k / 64;
            unsigned sh = (unsigned)(k % 64);
            rg_add_at(words, W, wi, e << sh);
            if (sh) rg_add_at(words, W, wi + 1, e >> (64 - sh));
        }
        uint64_t h1 = 1469598103934665603ULL, h2 = 0x9E3779B97F4A7C15ULL;
        for (i = 0; i < W; i++) {
            h1 = (h1 ^ words[i]) * 1099511628211ULL;
            h1 ^= h1 >> 29;
            h2 = (h2 + words[i]) * 0xBF58476D1CE4E5B9ULL;
            h2 ^= h2 >> 31;
        }
        size_t pos = (size_t)(h1 ^ (h2 >> 7)) & (tsz - 1);
        while (tid[pos] != 0 && !(th1[pos] == h1 && th2[pos] == h2))
            pos = (pos + 1) & (tsz - 1);
        if (tid[pos] == 0) {
            next_id++;
            tid[pos] = next_id;
            th1[pos] = h1;
            th2[pos] = h2;
        }
        ids[m] = tid[pos];
    }
    free(spf); free(pidx); free(th1); free(th2); free(tid); free(words);
    if (rg_id) free(rg_id);
    rg_id = ids;
    rg_limit = L;
}

seq_t a(idx_t n)
{
    if ((size_t)n > rg_limit) {
        size_t L = rg_limit ? rg_limit : 1024;
        while (L < (size_t)n) L *= 2;
        rg_build(L);
    }
    return (seq_t)rg_id[(size_t)n];
}
