/*
 * A391429  Odd cubefree exponential abundant numbers: odd cubefree numbers k
 *          for which A051377(k) > 2*k.
 * https://oeis.org/A391429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th odd cubefree k with A051377(k) > 2k, i.e. prod over
 *            p^2||k of (1+1/p) > 2. Computed by DFS over the sets of primes
 *            squared, then squarefree odd cofactors; limit doubled until n
 *            terms exist
 * Range:     u8 none, u16 none, u32 n <= 26, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 9670 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *g_terms = 0;
static size_t g_cnt = 0, g_cap = 0;
static uint64_t g_X = 0;      /* limit for which g_terms is complete */
static uint64_t *g_pr = 0;
static size_t g_np = 0;
static uint64_t cur_X;

static void p_push(uint64_t v)
{
    if (g_cnt == g_cap) {
        g_cap = g_cap ? g_cap * 2 : 1024;
        g_terms = (uint64_t *)realloc(g_terms, g_cap * sizeof(uint64_t));
    }
    g_terms[g_cnt++] = v;
}

static uint64_t p_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static bool p_sqfree(uint64_t m)
{
    for (uint64_t d = 3; d * d <= m; d += 2)
        if (m % (d * d) == 0) return false;
    return true;
}

static bool p_feasible(size_t j, uint64_t num, uint64_t den, uint64_t c)
{
    double r = (double)num / (double)den;
    uint64_t budget = cur_X / c;
    for (size_t k = j; k < g_np; k++) {
        uint64_t p = g_pr[k];
        if (p * p > budget) break;
        budget /= p * p;
        r *= (double)(p + 1) / (double)p;
        if (r > 2.0 - 1e-9) return true;
    }
    return r > 2.0 - 1e-9;
}

static void p_dfs(size_t start, uint64_t den, uint64_t num, uint64_t c)
{
    uint64_t lim = cur_X / c;
    for (size_t j = start; j < g_np; j++) {
        uint64_t p = g_pr[j];
        if (p * p > lim) break;
        uint64_t nden = den * p, nnum = num * (p + 1), nc = c * p * p;
        bool good = nnum > 2 * nden;
        if (!good && !p_feasible(j + 1, nnum, nden, nc)) {
            /* larger primes are no better */
            break;
        }
        if (good) {
            uint64_t mx = cur_X / nc;
            for (uint64_t m = 1; m <= mx; m += 2)
                if (p_gcd(m, nden) == 1 && p_sqfree(m))
                    p_push(nc * m);
        }
        p_dfs(j + 1, nden, nnum, nc);
    }
}

static int p_cmp(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return x < y ? -1 : (x > y);
}

static void p_build(uint64_t X)
{
    uint64_t L = 10;
    while (L * L <= X) L++;
    L = L / 3 + 10;
    char *comp = (char *)calloc(L + 1, 1);
    free(g_pr);
    g_pr = (uint64_t *)malloc((L + 1) * sizeof(uint64_t));
    g_np = 0;
    for (uint64_t i = 2; i <= L; i++) {
        if (!comp[i]) {
            if (i > 2) g_pr[g_np++] = i;
            for (uint64_t k = i * i; k <= L; k += i) comp[k] = 1;
        }
    }
    free(comp);
    cur_X = X;
    g_cnt = 0;
    p_dfs(0, 1, 1, 1);
    qsort(g_terms, g_cnt, sizeof(uint64_t), p_cmp);
    g_X = X;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    while (g_X == 0 || g_cnt < (size_t)n)
        p_build(g_X == 0 ? ((uint64_t)1 << 34) : g_X * 2);
    return (seq_t)g_terms[(size_t)(n - 1)];
}
