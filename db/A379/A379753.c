/*
 * A379753  Numbers that set records in A379752.
 * https://oeis.org/A379753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th record-setting k in A379752's underlying count c(k)
 *            (k over A375055); records only occur at A025487 numbers, so:
 *            search over A025487 numbers (exponents nonincreasing) up to a
 *            growing limit, c(k) from exponent-signature DP, take strict
 *            running maxima
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 73, u64 n <= 221, u128 n <= 226,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 203 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static uint64_t g_primes[64];
static bool g_primes_init = false;

static void rec_init_primes(void)
{
    if (g_primes_init) return;
    int cnt = 0;
    uint64_t q = 2;
    while (cnt < 64) {
        bool ok = true;
        for (int i = 0; i < cnt; i++) {
            if (q % g_primes[i] == 0) { ok = false; break; }
        }
        if (ok) g_primes[cnt++] = q;
        q++;
    }
    g_primes_init = true;
}

/* Number of qualifying pairs for the exponent signature exps[0..len). */
static uint64_t rec_count(const int *exps, int len)
{
    uint64_t dp[8], nd[8];
    for (int s = 0; s < 8; s++) dp[s] = 0;
    dp[0] = 1;
    for (int i = 0; i < len; i++) {
        uint64_t e = (uint64_t)exps[i];
        for (int s = 0; s < 8; s++) nd[s] = 0;
        for (int s = 0; s < 8; s++) {
            if (!dp[s]) continue;
            nd[s | 4] += dp[s];
            nd[s | 2] += dp[s];
            if (e >= 2) nd[s | 1] += dp[s] * (e - 1);
        }
        for (int s = 0; s < 8; s++) dp[s] = nd[s];
    }
    return dp[7] / 2;
}

/* generation state */
static seq_t *g_vals = NULL;
static uint64_t *g_cnts = NULL;
static size_t g_n = 0, g_cap = 0;
static int g_exps[64];
static seq_t g_limit;

static void rec_push(seq_t v, uint64_t c)
{
    if (g_n >= g_cap) {
        size_t nc = g_cap ? g_cap * 2 : 1024;
        g_vals = mem_resize(seq_t, g_vals, nc);
        g_cnts = (uint64_t *)realloc(g_cnts, nc * sizeof(uint64_t));
        g_cap = nc;
    }
    g_vals[g_n] = v;
    g_cnts[g_n] = c;
    g_n++;
}

static void rec_dfs(int idx, int maxe, seq_t v)
{
    if (idx > 0) {
        uint64_t c = rec_count(g_exps, idx);
        rec_push(v, c);
    }
    if (idx >= 64) return;
    seq_t p = (seq_t)g_primes[idx];
    seq_t cur = v;
    for (int e = 1; e <= maxe; e++) {
        if (cur > g_limit / p) break;
        cur = cur * p;
        g_exps[idx] = e;
        rec_dfs(idx + 1, e, cur);
    }
}

static const seq_t *g_sort_vals;
static int rec_cmp(const void *x, const void *y)
{
    size_t a1 = *(const size_t *)x, b1 = *(const size_t *)y;
    if (g_sort_vals[a1] < g_sort_vals[b1]) return -1;
    if (g_sort_vals[b1] < g_sort_vals[a1]) return 1;
    return 0;
}

static seq_t *r_res = NULL;
static size_t r_len = 0, r_cap = 0;
static bool r_init = false, r_exhausted = false;
static seq_t r_limit;

static void rec_run(seq_t L)
{
    g_limit = L;
    g_n = 0;
    rec_dfs(0, 200, (seq_t)1);
    size_t *idx = (size_t *)malloc((g_n ? g_n : 1) * sizeof(size_t));
    for (size_t i = 0; i < g_n; i++) idx[i] = i;
    g_sort_vals = g_vals;
    qsort(idx, g_n, sizeof(size_t), rec_cmp);
    r_len = 0;
    uint64_t best = 0;
    for (size_t i = 0; i < g_n; i++) {
        size_t j = idx[i];
        if (g_cnts[j] > best) {
            best = g_cnts[j];
            if (r_len >= r_cap) {
                r_cap = r_cap ? r_cap * 2 : 64;
                r_res = mem_resize(seq_t, r_res, r_cap);
            }
            r_res[r_len++] = g_vals[j];
        }
    }
    free(idx);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    rec_init_primes();
    if (!r_init) {
        r_init = true;
        r_limit = (seq_t)15;
        rec_run(r_limit);
    }
    while (r_len < (size_t)n && !r_exhausted) {
        seq_t L2 = r_limit * 16 + 15;
        if (L2 < r_limit || (L2 - 15) / 16 != r_limit) {
            r_exhausted = true;
            break;
        }
        r_limit = L2;
        rec_run(r_limit);
    }
    if (r_len < (size_t)n) return 0;
    return r_res[n - 1];
}
