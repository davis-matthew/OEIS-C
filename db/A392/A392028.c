/*
 * A392028  Indices of records in A255933.
 * https://oeis.org/A392028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   no closed form; this is a search. With s(i) =
 *            A255933(1)+...+A255933(i), A255933(i+1) = largest m with (m!-1)
 *            | s(i), and a(n) = the index where A255933 first exceeds every
 *            earlier term. A255933 is simulated with jumps. Values 2..6
 *            depend only on s mod L, where L = 5*23*119*719. A precomputed
 *            cyc...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A392028_L 9839515u   /* 5 * 23 * 119 * 719 */

static uint64_t A392028_dm[21];
static uint64_t A392028_rec[24];
static int A392028_rc = 0;
static uint64_t A392028_i = 0, A392028_s = 0, A392028_best = 0;
static uint32_t A392028_r = 0;
static bool A392028_inited = false, A392028_ph2 = false;

static int32_t *A392028_pos = NULL;
static uint8_t *A392028_wnd = NULL;
static int64_t *A392028_cs = NULL, *A392028_clen = NULL, *A392028_cW = NULL, *A392028_cx0 = NULL;
static int A392028_ncyc = 0, A392028_ccap = 0;

static uint64_t A392028_nm[21], A392028_M;
static uint64_t A392028_cand[21];
static int A392028_ckc[21];
static uint64_t A392028_ckp[21];

static uint64_t A392028_flow(uint32_t r)
{
    if (r % 719u == 0) return 6;
    if (r % 119u == 0) return 5;
    if (r % 23u == 0) return 4;
    if (r % 5u == 0) return 3;
    return 2;
}

static uint32_t A392028_next(uint32_t r)
{
    uint32_t nr = r + (uint32_t)A392028_flow(r);
    if (nr >= A392028_L) nr -= A392028_L;
    return nr;
}

static uint64_t A392028_ftrue(uint64_t s)
{
    for (int m = 20; m >= 3; --m)
        if (A392028_dm[m] <= s && s % A392028_dm[m] == 0) return (uint64_t)m;
    return 2;
}

static uint64_t A392028_ceilmult(uint64_t s, uint64_t d)
{
    return ((s + d - 1) / d) * d;
}

static void A392028_addcycle(int64_t start, int64_t len, int64_t W, int64_t x0)
{
    if (A392028_ncyc == A392028_ccap) {
        int nc = A392028_ccap ? A392028_ccap * 2 : 16;
        A392028_cs = (int64_t *)realloc(A392028_cs, sizeof(int64_t) * (size_t)nc);
        A392028_clen = (int64_t *)realloc(A392028_clen, sizeof(int64_t) * (size_t)nc);
        A392028_cW = (int64_t *)realloc(A392028_cW, sizeof(int64_t) * (size_t)nc);
        A392028_cx0 = (int64_t *)realloc(A392028_cx0, sizeof(int64_t) * (size_t)nc);
        if (!A392028_cs || !A392028_clen || !A392028_cW || !A392028_cx0) abort();
        A392028_ccap = nc;
    }
    A392028_cs[A392028_ncyc] = start;
    A392028_clen[A392028_ncyc] = len;
    A392028_cW[A392028_ncyc] = W;
    A392028_cx0[A392028_ncyc] = x0;
    A392028_ncyc++;
}

static void A392028_build(void)
{
    const uint32_t L = A392028_L;
    A392028_pos = (int32_t *)malloc(sizeof(int32_t) * (size_t)L);
    A392028_wnd = (uint8_t *)malloc((size_t)L);
    if (!A392028_pos || !A392028_wnd) abort();
    for (uint32_t r = 0; r < L; ++r) { A392028_pos[r] = -2; A392028_wnd[r] = 0; }
    int32_t gcount = 0;
    for (uint32_t r0 = 0; r0 < L; ++r0) {
        if (A392028_pos[r0] != -2) continue;
        int32_t stamp = -(3 + (int32_t)r0);
        uint32_t x = r0;
        while (A392028_pos[x] == -2) {
            A392028_pos[x] = stamp;
            x = A392028_next(x);
        }
        if (A392028_pos[x] == stamp) {
            int64_t start = gcount;
            uint32_t y = x;
            int w = 0;
            do {
                A392028_pos[y] = gcount++;
                A392028_wnd[y] = (uint8_t)w;
                uint32_t ny = y + (uint32_t)A392028_flow(y);
                if (ny >= L) { ny -= L; w++; }
                y = ny;
            } while (y != x);
            A392028_addcycle(start, (int64_t)gcount - start, (int64_t)w, (int64_t)x);
        }
        x = r0;
        while (A392028_pos[x] == stamp) {
            A392028_pos[x] = -1;
            x = A392028_next(x);
        }
    }
}

static int A392028_cycof(int32_t g)
{
    int lo = 0, hi = A392028_ncyc - 1;
    while (lo < hi) {
        int mid = (lo + hi + 1) / 2;
        if (A392028_cs[mid] <= (int64_t)g) lo = mid; else hi = mid - 1;
    }
    return lo;
}

static bool A392028_visited(uint64_t t, int c, uint64_t ph, uint64_t WL)
{
    uint32_t rt = (uint32_t)(t % A392028_L);
    int32_t gt = A392028_pos[rt];
    if (gt < 0) return false;
    if (A392028_cycof(gt) != c) return false;
    int64_t ut = (int64_t)rt - A392028_cx0[c] + (int64_t)A392028_wnd[rt] * (int64_t)A392028_L;
    return t % WL == (ph + (uint64_t)ut) % WL;
}

static void A392028_update_nm(void)
{
    uint64_t mn = UINT64_MAX;
    for (int m = 7; m <= 20; ++m) {
        if (A392028_nm[m] < A392028_s)
            A392028_nm[m] = A392028_ceilmult(A392028_s, A392028_dm[m]);
        if (A392028_nm[m] < mn) mn = A392028_nm[m];
    }
    A392028_M = mn;
}

static void A392028_jump(void)
{
    const uint64_t L = A392028_L;
    uint64_t s = A392028_s;
    int32_t g = A392028_pos[A392028_r];
    int c = A392028_cycof(g);
    int64_t x0 = A392028_cx0[c];
    uint64_t WL = (uint64_t)A392028_cW[c] * L;
    int64_t us = (int64_t)A392028_r - x0 + (int64_t)A392028_wnd[A392028_r] * (int64_t)L;
    uint64_t ph = (s % WL + WL - (uint64_t)us) % WL;
    uint64_t E = UINT64_MAX;
    for (int m = 7; m <= 20; ++m) {
        uint64_t d = A392028_dm[m];
        if (A392028_ckc[m] != c || A392028_ckp[m] != ph || A392028_cand[m] < s) {
            A392028_cand[m] = A392028_ceilmult(s, d);
            A392028_ckc[m] = c;
            A392028_ckp[m] = ph;
        }
        while (A392028_cand[m] < E && !A392028_visited(A392028_cand[m], c, ph, WL)) {
            if (A392028_cand[m] > UINT64_MAX - d) { A392028_cand[m] = UINT64_MAX; break; }
            A392028_cand[m] += d;
        }
        if (A392028_cand[m] < E) E = A392028_cand[m];
    }
    if (E == UINT64_MAX) abort();
    uint32_t rE = (uint32_t)(E % L);
    int32_t gE = A392028_pos[rE];
    int64_t uE = (int64_t)rE - x0 + (int64_t)A392028_wnd[rE] * (int64_t)L;
    int64_t q = ((int64_t)(E - s) - (uE - us)) / (int64_t)WL;
    int64_t steps = ((int64_t)gE - (int64_t)g) + q * A392028_clen[c];
    A392028_i += (uint64_t)steps;
    A392028_s = E;
    A392028_r = rE;
}

static void A392028_init(void)
{
    uint64_t f = 1;
    for (int m = 1; m <= 20; ++m) {
        f *= (uint64_t)m;
        A392028_dm[m] = f - 1;
    }
    for (int m = 0; m <= 20; ++m) { A392028_ckc[m] = -1; A392028_ckp[m] = 0; A392028_cand[m] = 0; }
    A392028_rec[0] = 1;
    A392028_rc = 1;
    A392028_i = 1;
    A392028_s = 1;
    A392028_best = 1;
    A392028_inited = true;
}

seq_t a(idx_t n)
{
    if (n == 0 || n > 20) return (seq_t)0;
    if (!A392028_inited) A392028_init();
    /* Phase 1: plain simulation until value 6 has appeared. */
    while (A392028_rc < (int)n && A392028_best < 6) {
        uint64_t v = A392028_ftrue(A392028_s);
        A392028_i++;
        if (v > A392028_best) { A392028_best = v; A392028_rec[A392028_rc++] = A392028_i; }
        A392028_s += v;
    }
    if (A392028_rc >= (int)n) return (seq_t)A392028_rec[n - 1];
    if (!A392028_ph2) {
        A392028_build();
        A392028_r = (uint32_t)(A392028_s % A392028_L);
        for (int m = 7; m <= 20; ++m)
            A392028_nm[m] = A392028_ceilmult(A392028_s, A392028_dm[m]);
        A392028_M = 0;
        A392028_update_nm();
        A392028_ph2 = true;
    }
    while (A392028_rc < (int)n) {
        if (A392028_s > A392028_M) A392028_update_nm();
        uint64_t v;
        if (A392028_s == A392028_M) {
            v = 2;
            for (int m = 20; m >= 7; --m)
                if (A392028_nm[m] == A392028_s) { v = (uint64_t)m; break; }
        } else {
            int32_t g = A392028_pos[A392028_r];
            if (g < 0) v = A392028_flow(A392028_r);
            else { A392028_jump(); continue; }
        }
        A392028_i++;
        if (v > A392028_best) { A392028_best = v; A392028_rec[A392028_rc++] = A392028_i; }
        A392028_s += v;
        A392028_r += (uint32_t)v;
        if (A392028_r >= A392028_L) A392028_r -= A392028_L;
    }
    return (seq_t)A392028_rec[n - 1];
}
