/*
 * A349065  Odd numbers k such that A183097(k) > 2*k.
 * https://oeis.org/A349065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th odd k with A183097(k) > 2*k, where A183097(k) =
 *            Prod_{p^e||k, e>=2} (1+p^2+...+p^e). Computed by search:
 *            enumerate odd powerful m, then squarefree odd r coprime to m
 *            with 2*m*r < A183097(m), k = m*r <= L; limit L doubles until n
 *            terms are found
 * Range:     u8 none, u16 none, u32 n <= 68, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 100 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *g_res = 0;
static size_t g_cnt = 0, g_cap = 0;
static uint64_t g_lim = 0;
static uint64_t *g_pr = 0;
static size_t g_np = 0;
static uint64_t g_L;

static void a349065_push(uint64_t k)
{
    if (g_cnt == g_cap) {
        g_cap = g_cap ? g_cap * 2 : 1024;
        g_res = (uint64_t *)realloc(g_res, g_cap * sizeof(uint64_t));
    }
    g_res[g_cnt++] = k;
}

static bool a349065_sqf_coprime(uint64_t r, uint64_t m)
{
    for (uint64_t p = 3; p * p <= r; p += 2) {
        if (r % p == 0) {
            r /= p;
            if (r % p == 0) return false;
            if (m % p == 0) return false;
        }
    }
    if (r > 1 && m % r == 0) return false;
    return true;
}

static void a349065_dfs(size_t idx, uint64_t m, uint64_t s)
{
    if (m > 1) {
        for (uint64_t r = 1; r <= g_L / m && 2 * m * r < s; r += 2) {
            if (r == 1 || a349065_sqf_coprime(r, m))
                a349065_push(m * r);
        }
    }
    for (size_t i = idx; i < g_np; ++i) {
        uint64_t p = g_pr[i];
        if (p * p > g_L / m) break;
        uint64_t pe = p, t = 1;
        uint64_t lim = g_L / m;
        for (;;) {
            if (pe > lim / p) break;
            pe *= p;
            t += pe;
            a349065_dfs(i + 1, m * pe, s * t);
        }
    }
}

static int a349065_cmp(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return a < b ? -1 : (a > b);
}

static void a349065_build(uint64_t L)
{
    uint64_t sq = 1;
    while (sq * sq <= L) sq++;
    sq++;
    char *comp = (char *)calloc(sq + 1, 1);
    free(g_pr);
    g_pr = (uint64_t *)malloc((sq + 1) * sizeof(uint64_t));
    g_np = 0;
    for (uint64_t i = 3; i <= sq; i += 2) {
        if (!comp[i]) {
            g_pr[g_np++] = i;
            for (uint64_t j = i * i; j <= sq; j += 2 * i) comp[j] = 1;
        }
    }
    free(comp);
    g_cnt = 0;
    g_L = L;
    a349065_dfs(0, 1, 1);
    qsort(g_res, g_cnt, sizeof(uint64_t), a349065_cmp);
    g_lim = L;
}

seq_t a(idx_t n)
{
    if (g_lim == 0) a349065_build(1000000000ULL);
    while ((uint64_t)g_cnt < (uint64_t)n)
        a349065_build(g_lim * 2);
    return (seq_t)g_res[n - 1];
}
