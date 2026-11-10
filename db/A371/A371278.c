/*
 * A371278  Numbers k > 1 such that k / A054841(k) is an integer.
 * https://oeis.org/A371278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed formula. Write k = Product p_j^e_j, with
 *            p_j the j-th prime (j starting at 1). Then A054841(k) = Sum
 *            e_j*10^(j-1), and k is a term when A054841(k) divides k. The
 *            code walks every factorization of k up to a limit, keeps the k
 *            that pass, sorts them, and raises the limit until t...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

#define NPR 40
static const unsigned pr_tab[NPR] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,
                                     73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173};

static seq_t *g_res = 0;
static size_t g_cnt = 0, g_cap = 0;
static seq_t g_lim;
static seq_t g_p10[NPR];
static int g_np10;

static void push_res(seq_t v) {
    if (g_cnt == g_cap) {
        size_t nc = g_cap ? g_cap * 2 : 64;
        if (g_res) g_res = mem_resize(seq_t, g_res, nc);
        else g_res = mem_new(seq_t, nc);
        g_cap = nc;
    }
    g_res[g_cnt++] = v;
}

static void dfs(int i, seq_t k, seq_t f) {
    if (k > 1 && f > 0 && k % f == 0) push_res(k);
    for (int j = i; j < g_np10; j++) {
        seq_t p = (seq_t)pr_tab[j];
        if (k > g_lim / p) break;
        seq_t kk = k, ff = f;
        for (;;) {
            if (kk > g_lim / p) break;
            kk *= p;
            ff += g_p10[j];
            if (ff > g_lim) break;
            dfs(j + 1, kk, ff);
        }
    }
}

static void build(seq_t L) {
    g_lim = L;
    g_cnt = 0;
    g_p10[0] = 1;
    g_np10 = 1;
    while (g_np10 < NPR && g_p10[g_np10 - 1] <= L / 10) {
        g_p10[g_np10] = g_p10[g_np10 - 1] * 10;
        g_np10++;
    }
    dfs(0, 1, 0);
    size_t gap = g_cnt / 2;
    while (gap > 0) {
        for (size_t i = gap; i < g_cnt; i++) {
            seq_t t = g_res[i];
            size_t j = i;
            while (j >= gap && g_res[j - gap] > t) {
                g_res[j] = g_res[j - gap];
                j -= gap;
            }
            g_res[j] = t;
        }
        gap /= 2;
    }
}

seq_t a(idx_t n) {
    static seq_t cur = 0;
    static bool at_max = false;
    if (cur != 0 && (idx_t)g_cnt >= n) return g_res[(size_t)(n - 1)];
    if (at_max) return 0;
    seq_t maxv = (seq_t)0 - 1;
    bool bounded = maxv > 0;
    seq_t L = cur != 0 ? cur * 16 : (seq_t)16;
    for (;;) {
        build(L);
        cur = L;
        if ((idx_t)g_cnt >= n) return g_res[(size_t)(n - 1)];
        if (bounded) {
            if (L == maxv) { at_max = true; return 0; }
            if (L > maxv / 16) L = maxv;
            else L *= 16;
        } else {
            L *= 16;
        }
    }
}
