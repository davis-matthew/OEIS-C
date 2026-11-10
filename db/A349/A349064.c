/*
 * A349064  Numbers k such that A183097(k) > 2*k.
 * https://oeis.org/A349064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A183097(k) > 2*k; computed by enumerating
 *            powerful numbers (DFS over prime powers) with a doubling search
 *            limit
 * Range:     u8 n <= 2, u16 n <= 113, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 8810 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A349064_terms = 0;
static size_t A349064_cnt = 0, A349064_cap = 0;
static uint32_t *A349064_pr = 0;
static size_t A349064_np = 0;
static uint64_t A349064_lim = 0;
static uint64_t A349064_have = 0;

static void A349064_push(uint64_t v)
{
    if (A349064_cnt == A349064_cap) {
        A349064_cap = A349064_cap ? A349064_cap * 2 : 1024;
        A349064_terms = (uint64_t *)realloc(A349064_terms, A349064_cap * sizeof(uint64_t));
    }
    A349064_terms[A349064_cnt++] = v;
}

static void A349064_dfs(uint64_t m, uint64_t f, size_t idx)
{
    for (size_t i = idx; i < A349064_np; ++i) {
        uint64_t p = A349064_pr[i];
        if (p * p > A349064_lim / m) break;
        uint64_t pe = p * p;
        uint64_t s = 1 + pe;
        uint64_t mm = m * pe;
        for (;;) {
            uint64_t nf = f * s;
            if (nf > 2 * mm) A349064_push(mm);
            A349064_dfs(mm, nf, i + 1);
            if (mm > A349064_lim / p) break;
            pe *= p;
            mm *= p;
            s += pe;
        }
    }
}

static int A349064_cmp(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return a < b ? -1 : (a > b);
}

static void A349064_build(uint64_t lim)
{
    uint64_t r = 1;
    while (r * r <= lim) ++r;
    size_t n = (size_t)r + 1;
    unsigned char *comp = (unsigned char *)calloc(n + 1, 1);
    free(A349064_pr);
    A349064_pr = (uint32_t *)malloc((n + 1) * sizeof(uint32_t));
    A349064_np = 0;
    for (size_t i = 2; i <= n; ++i) {
        if (!comp[i]) {
            A349064_pr[A349064_np++] = (uint32_t)i;
            for (size_t j = i * i; j <= n; j += i) comp[j] = 1;
        }
    }
    free(comp);
    A349064_cnt = 0;
    A349064_lim = lim;
    A349064_dfs(1, 1, 0);
    qsort(A349064_terms, A349064_cnt, sizeof(uint64_t), A349064_cmp);
    A349064_have = lim;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A349064_have == 0) A349064_build((uint64_t)1 << 20);
    while ((uint64_t)A349064_cnt < (uint64_t)n) {
        A349064_build(A349064_have * 4);
    }
    return (seq_t)A349064_terms[n - 1];
}
