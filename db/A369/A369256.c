/*
 * A369256  Positions of odd terms in A140773.
 * https://oeis.org/A369256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k-th number m such that A140773(m) is odd, where
 *            A140773(m) = Sum_{d|m} ceil(tau(d)/2); found by a sequential
 *            scan with memoization
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static uint64_t A369256_dfs(const uint8_t *exps, size_t idx, size_t cnt, uint64_t d_val)
{
    if (idx == cnt) return (d_val + 1) >> 1;
    uint64_t total = 0;
    for (unsigned f = 0; f <= exps[idx]; ++f)
        total += A369256_dfs(exps, idx + 1, cnt, d_val * (uint64_t)(f + 1));
    return total;
}

static uint64_t A369256_A140773(uint64_t n)
{
    uint8_t exps[64];
    size_t cnt = 0;
    uint64_t t = n;
    for (uint64_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            uint8_t e = 0;
            while (t % p == 0) { t /= p; ++e; }
            exps[cnt++] = e;
        }
    }
    if (t > 1) exps[cnt++] = 1;
    return A369256_dfs(exps, 0, cnt, 1);
}

static uint64_t *A369256_cache = NULL;
static size_t A369256_cnt = 0, A369256_cap = 0;
static uint64_t A369256_next = 1;

seq_t a(idx_t n)
{
    size_t want = (size_t)n;
    while (A369256_cnt < want) {
        uint64_t k = A369256_next++;
        if (A369256_A140773(k) & 1ULL) {
            if (A369256_cnt == A369256_cap) {
                size_t nc = A369256_cap ? A369256_cap * 2 : 1024;
                A369256_cache = (uint64_t *)realloc(A369256_cache, nc * sizeof(uint64_t));
                A369256_cap = nc;
            }
            A369256_cache[A369256_cnt++] = k;
        }
    }
    return (seq_t)A369256_cache[want - 1];
}
