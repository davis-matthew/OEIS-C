/*
 * A306304  Least number m such that A323835(m) = n.
 * https://oeis.org/A306304
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   search. Tabulate the step count A323835 for every number with
 *            distinct decimal digits, then a(n) = (least even N >= 4 with
 *            A323835(A320485(N)) = n-1) / 2, where N is built from each such
 *            target y. a(-1) and a(0) come from a direct scan
 * Range:     u8 n <= 52, u16 n <= 59, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 64 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static uint64_t A306304_P[11][11];
static uint64_t A306304_off[12];
static uint64_t A306304_p10[20];
static int16_t *A306304_T = NULL;
static uint32_t *A306304_stk = NULL;
static size_t A306304_stk_cap = 0;

/* A320485: keep only the digits that occur exactly once */
static uint64_t A306304_del(uint64_t m)
{
    unsigned cnt[10] = {0};
    unsigned char dg[24];
    int nd = 0;
    uint64_t t = m;
    if (t == 0) return 0;
    while (t > 0) {
        unsigned d = (unsigned)(t % 10);
        dg[nd++] = (unsigned char)d;
        cnt[d]++;
        t /= 10;
    }
    uint64_t r = 0;
    for (int i = nd - 1; i >= 0; --i)
        if (cnt[dg[i]] == 1) r = r * 10 + dg[i];
    return r;
}

/* rank of a number with distinct decimal digits (its index in increasing order) */
static uint32_t A306304_rank(uint64_t x)
{
    int d[24], L = 0;
    if (x == 0) return 0;
    while (x) { d[L++] = (int)(x % 10); x /= 10; }
    if (L == 1) return (uint32_t)d[0];
    uint64_t r = A306304_off[L];
    unsigned used = 0;
    for (int i = 0; i < L; i++) {
        int dg = d[L - 1 - i];
        int c;
        if (i == 0) c = dg - 1;
        else {
            c = 0;
            for (int j = 0; j < dg; j++) if (!((used >> j) & 1u)) c++;
        }
        r += (uint64_t)c * A306304_P[9 - i][L - 1 - i];
        used |= 1u << dg;
    }
    return (uint32_t)r;
}

static void A306304_push(size_t *sp, uint32_t r)
{
    if (*sp >= A306304_stk_cap) {
        size_t nc = A306304_stk_cap ? A306304_stk_cap * 2 : 1024;
        A306304_stk = (uint32_t *)realloc(A306304_stk, nc * sizeof(uint32_t));
        A306304_stk_cap = nc;
    }
    A306304_stk[(*sp)++] = r;
}

static void A306304_chain(uint64_t x)
{
    size_t sp = 0;
    int base;
    uint64_t v = x;
    for (;;) {
        uint32_t r = A306304_rank(v);
        int16_t t = A306304_T[r];
        if (t != -2) { base = (t == -3) ? -1 : t; break; }
        A306304_T[r] = -3;
        A306304_push(&sp, r);
        v = A306304_del(2 * v);
    }
    while (sp > 0) {
        uint32_t r = A306304_stk[--sp];
        if (base >= 0) base++;
        A306304_T[r] = (int16_t)base;
    }
}

static void A306304_gen(uint64_t x, unsigned used, int len)
{
    A306304_chain(x);
    if (len == 10) return;
    for (int d = 0; d < 10; d++)
        if (!((used >> d) & 1u))
            A306304_gen(x * 10 + (uint64_t)d, used | (1u << d), len + 1);
}

static int A306304_steps(uint64_t m)
{
    if (m <= 1) return 0;
    int t = A306304_T[A306304_rank(A306304_del(2 * m))];
    return t < 0 ? -1 : t + 1;
}

/* per-query search state */
static uint64_t A306304_best;
static int A306304_tgt;
static uint32_t A306304_ctr;

/* smallest even N of length k+2 whose unique digits spell s, using one extra digit twice */
static void A306304_merge(const int *s, int k)
{
    int L = k + 2;
    unsigned inS = 0;
    for (int i = 0; i < k; i++) inS |= 1u << s[i];
    for (int x = 0; x < 10; x++) {
        if ((inS >> x) & 1u) continue;
        for (int i = 0; i < L; i++)
            for (int j = i + 1; j < L; j++) {
                int d[16], p = 0;
                for (int q = 0; q < L; q++)
                    d[q] = (q == i || q == j) ? x : s[p++];
                if (d[0] == 0 || (d[L - 1] & 1)) continue;
                uint64_t v = 0;
                for (int q = 0; q < L; q++) v = v * 10 + (uint64_t)d[q];
                if (v >= 4 && v < A306304_best) A306304_best = v;
            }
    }
}

static void A306304_cand(uint64_t y, int k)
{
    int s[12], s0[12];
    bool hasZero = false;
    uint64_t t = y;
    for (int i = k - 1; i >= 0; i--) { s[i] = (int)(t % 10); t /= 10; if (s[i] == 0) hasZero = true; }
    if (y >= 4 && (y & 1) == 0 && y < A306304_best) A306304_best = y;
    if (A306304_best > A306304_p10[k + 1]) A306304_merge(s, k);
    if (y == 0 && A306304_best > A306304_p10[1]) A306304_merge(s, 0);
    if (!hasZero && A306304_best > A306304_p10[k + 2]) {
        s0[0] = 0;
        for (int i = 0; i < k; i++) s0[i + 1] = s[i];
        A306304_merge(s0, k + 1);
    }
}

static void A306304_walk(uint64_t y, unsigned used, int len, int L)
{
    if (len == L) {
        if (A306304_T[A306304_ctr++] == A306304_tgt) A306304_cand(y, L);
        return;
    }
    for (int d = 0; d < 10; d++)
        if (!((used >> d) & 1u))
            A306304_walk(y * 10 + (uint64_t)d, used | (1u << d), len + 1, L);
}

seq_t a(idx_t n)
{
    static bool built = false;
    static uint64_t ans[66];
    static bool have[66];
    if (n < -1 || n > 64) return (seq_t)0;
    if (!built) {
        built = true;
        for (int i = 0; i <= 10; i++) {
            A306304_P[i][0] = 1;
            for (int j = 1; j <= i; j++)
                A306304_P[i][j] = A306304_P[i][j - 1] * (uint64_t)(i - j + 1);
        }
        A306304_p10[0] = 1;
        for (int i = 1; i < 20; i++) A306304_p10[i] = A306304_p10[i - 1] * 10;
        A306304_off[1] = 0;
        A306304_off[2] = 10;
        for (int L = 2; L <= 10; L++)
            A306304_off[L + 1] = A306304_off[L] + 9 * A306304_P[9][L - 1];
        size_t N = (size_t)A306304_off[11];
        A306304_T = (int16_t *)malloc(N * sizeof(int16_t));
        for (size_t i = 0; i < N; i++) A306304_T[i] = -2;
        A306304_T[0] = 0;
        A306304_T[1] = 0;
        for (int d = 1; d <= 9; d++) A306304_gen((uint64_t)d, 1u << d, 1);
        free(A306304_stk);
        A306304_stk = NULL;
        A306304_stk_cap = 0;
        for (int i = 0; i < 66; i++) have[i] = false;
    }
    int k = (int)(n + 1);
    if (!have[k]) {
        uint64_t res = 0;
        if (n == 0) res = 0;
        else if (n == -1) {
            uint64_t m = 0;
            while (A306304_steps(m) != -1) m++;
            res = m;
        } else {
            A306304_best = UINT64_MAX;
            A306304_tgt = (int)(n - 1);
            for (int L = 1; L <= 10; L++) {
                if (A306304_best <= A306304_p10[L - 1]) break;
                A306304_ctr = (uint32_t)A306304_off[L];
                if (L == 1) {
                    for (int d = 0; d < 10; d++) {
                        if (A306304_T[A306304_ctr++] == A306304_tgt)
                            A306304_cand((uint64_t)d, 1);
                    }
                } else {
                    for (int d = 1; d <= 9; d++)
                        A306304_walk((uint64_t)d, 1u << d, 1, L);
                }
            }
            if (A306304_best != UINT64_MAX) res = A306304_best / 2;
        }
        ans[k] = res;
        have[k] = true;
    }
    return (seq_t)ans[k];
}
