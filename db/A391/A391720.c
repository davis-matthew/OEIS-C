/*
 * A391720  a(n) is the smallest k for which A243917(k) = n.
 * https://oeis.org/A391720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k with A243917(k) = n, found by search: DFS over
 *            prime factorizations of k <= B (doubling B) whose divisor count
 *            tau(k) >= n, pruned by tau * 2^(remaining prime factors) < n;
 *            for each candidate the non-twin divisors are counted exactly
 * Range:     u8 n <= 12, u16 n <= 75, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Validated: standard, pass, 64 terms through n = 493 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static uint64_t *A391720_pr = NULL;
static size_t A391720_pcnt = 0;
static uint64_t A391720_plim = 0;

static void A391720_extend(void)
{
    uint64_t lim = A391720_plim ? A391720_plim * 2 : 4096;
    unsigned char *comp = (unsigned char *)malloc((size_t)lim + 1);
    if (!comp) abort();
    memset(comp, 0, (size_t)lim + 1);
    size_t cnt = 0;
    for (uint64_t i = 2; i <= lim; i++) {
        if (!comp[i]) {
            cnt++;
            for (uint64_t j = i * i; j <= lim; j += i) comp[j] = 1;
        }
    }
    uint64_t *pr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!pr) abort();
    size_t c = 0;
    for (uint64_t i = 2; i <= lim; i++) if (!comp[i]) pr[c++] = i;
    free(comp);
    free(A391720_pr);
    A391720_pr = pr;
    A391720_pcnt = cnt;
    A391720_plim = lim;
}

static uint64_t A391720_lim, A391720_best, A391720_n;
static uint64_t A391720_ps[64];
static int A391720_es[64];
static int A391720_depth;
static uint64_t *A391720_div = NULL;
static size_t A391720_divcap = 0;

/* exact count of non-twin divisors of k, given its factorization on the stack */
static uint64_t A391720_count(uint64_t k, uint64_t tau)
{
    if (tau > A391720_divcap) {
        free(A391720_div);
        A391720_divcap = (size_t)tau * 2;
        A391720_div = (uint64_t *)malloc(A391720_divcap * sizeof(uint64_t));
        if (!A391720_div) abort();
    }
    size_t sz = 1;
    A391720_div[0] = 1;
    for (int l = 0; l < A391720_depth; l++) {
        uint64_t p = A391720_ps[l];
        size_t cur = sz;
        uint64_t mult = 1;
        for (int j = 1; j <= A391720_es[l]; j++) {
            mult *= p;
            for (size_t i = 0; i < cur; i++) A391720_div[sz++] = A391720_div[i] * mult;
        }
    }
    uint64_t twins = 0, maxtw = tau - A391720_n;
    uint64_t k2 = 2 * k;
    for (size_t i = 0; i < sz; i++) {
        uint64_t d = A391720_div[i];
        if (d > 2 && (d - 2) * (d - 2) > k2) continue; /* cannot be a twin */
        bool tw = false;
        if (d > 2 && k % (d - 2) == 0) tw = true;
        if (!tw && k % (d + 2) == 0) tw = true;
        if (tw) {
            twins++;
            if (twins > maxtw) return tau - twins;
        }
    }
    return tau - twins;
}

static void A391720_dfs(size_t idx, uint64_t m, uint64_t tau)
{
    if (tau >= A391720_n && m < A391720_best) {
        if (A391720_count(m, tau) == A391720_n) {
            A391720_best = m;
            A391720_lim = m - 1;
            return; /* everything below is a multiple of m, hence larger */
        }
    }
    for (size_t i = idx;; i++) {
        while (i >= A391720_pcnt) A391720_extend();
        uint64_t p = A391720_pr[i];
        if (m > A391720_lim / p) break;
        uint64_t R = A391720_lim / m;
        uint64_t q = p;
        int t = 1;
        while (q <= R / p) { q *= p; t++; }
        uint64_t v = tau;
        for (int j = 0; j < t && v < A391720_n; j++) v <<= 1;
        if (v < A391720_n) break;
        int d = A391720_depth;
        uint64_t cur = m;
        int e = 0;
        while (cur <= A391720_lim / p) {
            cur *= p;
            e++;
            A391720_ps[d] = p;
            A391720_es[d] = e;
            A391720_depth = d + 1;
            A391720_dfs(i + 1, cur, tau * (uint64_t)(e + 1));
            A391720_depth = d;
        }
    }
}

#define A391720_CACHE 8192
static uint64_t A391720_cache[A391720_CACHE];
static bool A391720_have[A391720_CACHE];

seq_t a(idx_t n)
{
    if (n < A391720_CACHE && A391720_have[n]) return (seq_t)A391720_cache[n];
    uint64_t B = 16;
    uint64_t res = 0;
    for (;;) {
        A391720_n = (uint64_t)n;
        A391720_lim = B;
        A391720_best = UINT64_MAX;
        A391720_depth = 0;
        A391720_dfs(0, 1, 1);
        if (A391720_best != UINT64_MAX) { res = A391720_best; break; }
        B *= 2;
    }
    if (n < A391720_CACHE) { A391720_cache[n] = res; A391720_have[n] = true; }
    return (seq_t)res;
}
