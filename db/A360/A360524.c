/*
 * A360524  Numbers k such that A360522(k) = 2*k.
 * https://oeis.org/A360524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th k such that A360522(k) = 2*k, where A360522(k) =
 *            Product_{p^e||k} (p^e + e); found by DFS over prime-power
 *            factorizations with ratio pruning
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 24, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static uint64_t *g_pr = 0;
static size_t g_np = 0;
static uint64_t g_lim = 0;
static uint64_t g_found[256];
static size_t g_nf = 0;
static uint64_t g_done_lim = 0;
static size_t g_done_cnt = 0;

static bool a360524_isprime(uint64_t q)
{
    size_t i;
    if (q < 2) return false;
    for (i = 0; i < g_np && g_pr[i] * g_pr[i] <= q; i++)
        if (q % g_pr[i] == 0) return false;
    return true;
}

static void a360524_rec(uint64_t m, uint64_t f)
{
    /* find index: handled via parameter-free trick below */
    (void)m; (void)f;
}

static void a360524_dfs(size_t i, uint64_t m, uint64_t f)
{
    uint64_t L = g_lim / m;
    uint64_t last = i > 0 ? g_pr[i - 1] : 1;
    size_t j;
    /* closure with a single new prime q (exponent 1) */
    if (2 * m > f) {
        uint64_t d = 2 * m - f;
        if (f % d == 0) {
            uint64_t q = f / d;
            if (q > last && q <= L && a360524_isprime(q) && g_nf < 256)
                g_found[g_nf++] = m * q;
        }
    }
    /* bound on the best achievable ratio with remaining budget */
    {
        double r = (double)f / (double)m;
        uint64_t prod = 1;
        size_t k = i;
        while (k < g_np && prod <= L / g_pr[k]) {
            prod *= g_pr[k];
            r *= 1.0 + 1.0 / (double)g_pr[k];
            k++;
        }
        if (r * (1.0 + 1e-9) < 2.0) return;
    }
    for (j = i; j + 1 < g_np && g_pr[j] * g_pr[j] <= L; j++) {
        uint64_t p = g_pr[j];
        uint64_t pe = p;
        uint64_t e = 1;
        while (pe <= L) {
            uint64_t nm = m * pe;
            uint64_t nf = f * (pe + e);
            if (e >= 2 && nf == 2 * nm && g_nf < 256)
                g_found[g_nf++] = nm;
            if (nf <= 2 * nm && nm <= g_lim / g_pr[j + 1])
                a360524_dfs(j + 1, nm, nf);
            if (pe > L / p) break;
            pe *= p;
            e++;
        }
    }
}

static int a360524_cmp(const void *x, const void *y)
{
    uint64_t a = *(const uint64_t *)x, b = *(const uint64_t *)y;
    return a < b ? -1 : (a > b ? 1 : 0);
}

static void a360524_run(uint64_t lim)
{
    uint64_t s = 1;
    size_t i, j;
    unsigned char *sv;
    while (s * s <= lim) s++;
    s += 1000;
    sv = (unsigned char *)calloc((size_t)s + 1, 1);
    if (g_pr) free(g_pr);
    g_pr = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)s + 1));
    g_np = 0;
    for (i = 2; i <= s; i++) {
        if (!sv[i]) {
            g_pr[g_np++] = i;
            for (j = i * i; j <= s; j += i) sv[j] = 1;
        }
    }
    free(sv);
    g_lim = lim;
    g_nf = 0;
    a360524_dfs(0, 1, 1);
    qsort(g_found, g_nf, sizeof(uint64_t), a360524_cmp);
    g_done_lim = lim;
    g_done_cnt = g_nf;
}

seq_t a(idx_t n)
{
    uint64_t lim;
    if (n < 1 || n > 200) return (seq_t)0;
    if (g_done_lim == 0) lim = 10000;
    else lim = g_done_lim;
    while (g_done_lim == 0 || g_done_cnt < (size_t)n) {
        if (g_done_lim != 0) lim = g_done_lim * 8;
        if (lim > 20000000000000ULL) return (seq_t)0;
        a360524_run(lim);
    }
    (void)a360524_rec;
    return (seq_t)g_found[n - 1];
}
