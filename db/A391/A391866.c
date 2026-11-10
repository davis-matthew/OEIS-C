/*
 * A391866  Nonsquarefree numbers k that are not divisible by p^p for any
 *          prime p, and for which A276085(k) is a multiple of A003557(k),
 *          where A276085 is the primorial base log-function.
 * https://oeis.org/A391866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A008966(k)=0, A359550(k)=1 and
 *            A003557(k) | A276085(k); computed by incremental search with an
 *            spf sieve
 * Range:     u8 n <= 1, u16 n <= 235, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9819 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static uint32_t *g_spf = NULL;
static uint32_t *g_pr = NULL;
static size_t g_npr = 0;
static uint64_t g_lim = 0;
static uint64_t *g_terms = NULL;
static size_t g_cnt = 0, g_cap = 0;
static uint64_t g_next = 2;

static void rebuild_sieve(uint64_t lim)
{
    free(g_spf);
    free(g_pr);
    g_spf = (uint32_t *)calloc((size_t)lim + 1, sizeof(uint32_t));
    g_pr = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)lim / 2 + 16));
    g_npr = 0;
    for (uint64_t i = 2; i <= lim; ++i) {
        if (g_spf[i] == 0) {
            g_spf[i] = (uint32_t)i;
            g_pr[g_npr++] = (uint32_t)i;
        }
        for (size_t j = 0; j < g_npr; ++j) {
            uint64_t p = g_pr[j];
            if (p > g_spf[i] || p * i > lim) break;
            g_spf[p * i] = (uint32_t)p;
        }
    }
    g_lim = lim;
}

static int test_k(uint64_t k)
{
    uint64_t fp[16], fe[16];
    int nf = 0;
    uint64_t t = k;
    int has_sq = 0;
    while (t > 1) {
        uint64_t p = g_spf[t];
        uint64_t e = 0;
        while (t % p == 0) { t /= p; ++e; }
        if (e >= p) return 0;
        if (e >= 2) has_sq = 1;
        fp[nf] = p; fe[nf] = e; ++nf;
    }
    if (!has_sq) return 0;

    uint64_t m = 1;
    int all_simple = 1;
    for (int i = 0; i < nf; ++i) {
        for (uint64_t j = 1; j < fe[i]; ++j) m *= fp[i];
        if (fe[i] > 2) all_simple = 0;
    }

    /* necessary condition modulo each prime q dividing m */
    for (int i = 0; i < nf; ++i) {
        if (fe[i] < 2) continue;
        uint64_t q = fp[i];
        uint64_t prod = 1 % q, s = 0;
        for (size_t j = 0; j < g_npr && g_pr[j] <= q; ++j) {
            uint64_t r = g_pr[j];
            for (int u = 0; u < nf; ++u) {
                if (fp[u] == r) { s = (s + fe[u] * prod) % q; break; }
            }
            prod = prod * r % q;
        }
        if (s != 0) return 0;
    }
    if (all_simple) return 1;

    /* full check modulo m */
    uint64_t maxp = fp[nf - 1];
    unsigned __int128 prod = 1 % m, s = 0;
    for (size_t j = 0; j < g_npr && g_pr[j] <= maxp; ++j) {
        uint64_t r = g_pr[j];
        for (int u = 0; u < nf; ++u) {
            if (fp[u] == r) { s = (s + (unsigned __int128)fe[u] * prod) % m; break; }
        }
        prod = prod * r % m;
    }
    return s == 0;
}

seq_t a(idx_t n)
{
    while (g_cnt < (size_t)n) {
        if (g_next > g_lim) {
            uint64_t nl = g_lim ? g_lim * 2 : (1u << 16);
            rebuild_sieve(nl);
        }
        uint64_t k = g_next++;
        if (test_k(k)) {
            if (g_cnt == g_cap) {
                g_cap = g_cap ? g_cap * 2 : 1024;
                g_terms = (uint64_t *)realloc(g_terms, g_cap * sizeof(uint64_t));
            }
            g_terms[g_cnt++] = k;
        }
    }
    return (seq_t)g_terms[(size_t)n - 1];
}
