/*
 * A387078  Run lengths of A386482(n) mod 2 == n mod 2.
 * https://oeis.org/A387078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the n-th maximal run of equal parity in S =
 *            A386482. Computed by generating S directly: for each prime p, a
 *            64-ary bit tree over the multiples of p finds the greatest
 *            unused multiple below a(n-1) (or the smallest one above it) in
 *            O(log) time; S(1) = 1, S(2) = 2
 * Range:     u8 n <= 69, u16 n <= 161, u32 n <= 260, u64 n <= 260,
 *            u128 n <= 260, big any n
 * Validated: standard, pass, 89 terms through n = 255 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define A387078_T 65536u
#define A387078_L 5

typedef struct {
    uint64_t *lv[A387078_L];
    size_t nw[A387078_L];
    bool made;
} A387078_tree;

static uint32_t *A387078_pl = NULL;      /* primes <= T */
static size_t A387078_np = 0;
static uint16_t *A387078_pidx = NULL;    /* prime -> index */
static A387078_tree *A387078_trees = NULL;
static uint16_t *A387078_spf = NULL;     /* index+1 of smallest prime factor, 0 = prime */
static size_t A387078_spf_lim = 0;
static uint64_t *A387078_ub = NULL;      /* used bitset */
static size_t A387078_ubw = 0;
static uint64_t *A387078_runs = NULL;
static size_t A387078_runs_len = 0, A387078_runs_cap = 0;
static uint64_t A387078_prev = 0, A387078_cur = 0;
static bool A387078_init = false;

static void A387078_tr_grow(A387078_tree *t, uint64_t j)
{
    size_t need0 = (size_t)(j / 64) + 1, nw, cur;
    int l;
    if (need0 <= t->nw[0]) return;
    nw = t->nw[0] ? t->nw[0] : 4;
    while (nw < need0) nw *= 2;
    cur = nw;
    for (l = 0; l < A387078_L; l++) {
        if (cur > t->nw[l]) {
            uint64_t *p = (uint64_t *)realloc(t->lv[l], cur * sizeof(uint64_t));
            if (!p) abort();
            memset(p + t->nw[l], 0, (cur - t->nw[l]) * sizeof(uint64_t));
            t->lv[l] = p;
            t->nw[l] = cur;
        }
        cur = (cur + 63) / 64;
    }
}

static void A387078_tr_set(A387078_tree *t, uint64_t j)
{
    int l;
    A387078_tr_grow(t, j);
    for (l = 0; l < A387078_L; l++) {
        uint64_t w = j >> 6;
        t->lv[l][w] |= 1ULL << (j & 63);
        if (t->lv[l][w] != ~0ULL) return;
        j = w;
    }
}

static int64_t A387078_tr_prev(A387078_tree *t, int l, int64_t j)
{
    int64_t w, w2;
    uint64_t m;
    int b;
    if (j < 0) return -1;
    if ((uint64_t)j >= (uint64_t)t->nw[l] * 64) return j;
    w = j >> 6; b = (int)(j & 63);
    m = ~t->lv[l][w];
    if (b < 63) m &= (2ULL << b) - 1;
    if (m) return w * 64 + 63 - __builtin_clzll(m);
    if (w == 0) return -1;
    if (l == A387078_L - 1) {
        w2 = w - 1;
        while (w2 >= 0 && t->lv[l][w2] == ~0ULL) w2--;
        if (w2 < 0) return -1;
    } else {
        w2 = A387078_tr_prev(t, l + 1, w - 1);
        if (w2 < 0) return -1;
    }
    m = ~t->lv[l][w2];
    return w2 * 64 + 63 - __builtin_clzll(m);
}

static uint64_t A387078_tr_next(A387078_tree *t, int l, uint64_t j)
{
    uint64_t w, w2, m;
    if (j >= (uint64_t)t->nw[l] * 64) return j;
    w = j >> 6;
    m = ~t->lv[l][w] & (~0ULL << (j & 63));
    if (m) return w * 64 + __builtin_ctzll(m);
    if (l == A387078_L - 1) {
        w2 = w + 1;
        while (w2 < t->nw[l] && t->lv[l][w2] == ~0ULL) w2++;
    } else {
        w2 = A387078_tr_next(t, l + 1, w + 1);
    }
    if (w2 >= t->nw[l]) return w2 * 64;
    m = ~t->lv[l][w2];
    return w2 * 64 + __builtin_ctzll(m);
}

static A387078_tree *A387078_tree_of(uint64_t p)
{
    A387078_tree *t = &A387078_trees[A387078_pidx[p]];
    if (!t->made) {
        t->made = true;
        A387078_tr_set(t, 0);
    }
    return t;
}

static void A387078_spf_build(size_t lim)
{
    size_t i, m;
    free(A387078_spf);
    A387078_spf = (uint16_t *)calloc(lim, sizeof(uint16_t));
    if (!A387078_spf) abort();
    for (i = 0; i < A387078_np; i++) {
        uint64_t p = A387078_pl[i];
        if (p * p >= lim) break;
        for (m = (size_t)(p * p); m < lim; m += (size_t)p)
            if (!A387078_spf[m]) A387078_spf[m] = (uint16_t)(i + 1);
    }
    A387078_spf_lim = lim;
}

static int A387078_factor(uint64_t k, uint64_t *out)
{
    int c = 0;
    if (k >= A387078_spf_lim) {
        size_t nl = A387078_spf_lim * 2;
        if (nl < k + 1) nl = (size_t)k + 1;
        A387078_spf_build(nl);
    }
    while (k > 1 && A387078_spf[k]) {
        uint64_t p = A387078_pl[A387078_spf[k] - 1];
        out[c++] = p;
        while (k % p == 0) k /= p;
    }
    if (k > 1) out[c++] = k;
    return c;
}

static bool A387078_used(uint64_t k)
{
    size_t w = (size_t)(k >> 6);
    if (w >= A387078_ubw) return false;
    return (A387078_ub[w] >> (k & 63)) & 1;
}

static void A387078_mark(uint64_t k)
{
    size_t w = (size_t)(k >> 6);
    uint64_t fs[16];
    int c, i;
    if (w >= A387078_ubw) {
        size_t nw = A387078_ubw ? A387078_ubw : 1024;
        uint64_t *p;
        while (nw <= w) nw *= 2;
        p = (uint64_t *)realloc(A387078_ub, nw * sizeof(uint64_t));
        if (!p) abort();
        memset(p + A387078_ubw, 0, (nw - A387078_ubw) * sizeof(uint64_t));
        A387078_ub = p;
        A387078_ubw = nw;
    }
    A387078_ub[w] |= 1ULL << (k & 63);
    c = A387078_factor(k, fs);
    for (i = 0; i < c; i++)
        if (fs[i] <= A387078_T)
            A387078_tr_set(A387078_tree_of(fs[i]), k / fs[i]);
}

static uint64_t A387078_next(uint64_t x)
{
    uint64_t fs[16], best = 0;
    int c = A387078_factor(x, fs), i;
    for (i = 0; i < c; i++) {
        uint64_t p = fs[i];
        if (p <= A387078_T) {
            int64_t j = A387078_tr_prev(A387078_tree_of(p), 0, (int64_t)((x - 1) / p));
            if (j >= 1 && (uint64_t)j * p > best) best = (uint64_t)j * p;
        } else {
            uint64_t k = ((x - 1) / p) * p;
            while (k > best && k > 0) {
                if (!A387078_used(k)) { best = k; break; }
                k -= p;
            }
        }
    }
    if (best) return best;
    best = UINT64_MAX;
    for (i = 0; i < c; i++) {
        uint64_t p = fs[i];
        if (p <= A387078_T) {
            uint64_t j = A387078_tr_next(A387078_tree_of(p), 0, x / p + 1);
            if (j * p < best) best = j * p;
        } else {
            uint64_t k = (x / p + 1) * p;
            while (k < best) {
                if (!A387078_used(k)) { best = k; break; }
                k += p;
            }
        }
    }
    return best;
}

static void A387078_push_run(uint64_t len)
{
    if (A387078_runs_len == A387078_runs_cap) {
        size_t nc = A387078_runs_cap ? A387078_runs_cap * 2 : 64;
        uint64_t *t = (uint64_t *)realloc(A387078_runs, nc * sizeof(uint64_t));
        if (!t) abort();
        A387078_runs = t;
        A387078_runs_cap = nc;
    }
    A387078_runs[A387078_runs_len++] = len;
}

static void A387078_setup(void)
{
    uint8_t *comp = (uint8_t *)calloc(A387078_T + 1, 1);
    uint32_t i, j;
    if (!comp) abort();
    A387078_pl = (uint32_t *)malloc(sizeof(uint32_t) * 7000);
    A387078_pidx = (uint16_t *)calloc(A387078_T + 1, sizeof(uint16_t));
    if (!A387078_pl || !A387078_pidx) abort();
    for (i = 2; i <= A387078_T; i++) {
        if (comp[i]) continue;
        A387078_pidx[i] = (uint16_t)A387078_np;
        A387078_pl[A387078_np++] = i;
        for (j = i * 2; j <= A387078_T; j += i) comp[j] = 1;
    }
    free(comp);
    A387078_trees = (A387078_tree *)calloc(A387078_np, sizeof(A387078_tree));
    if (!A387078_trees) abort();
    A387078_spf_build((size_t)1 << 16);
    A387078_mark(1);
    A387078_prev = 1;
    A387078_cur = 1;
    A387078_init = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!A387078_init) A387078_setup();
    while (A387078_runs_len < (size_t)n) {
        uint64_t nx = (A387078_prev == 1) ? 2 : A387078_next(A387078_prev);
        A387078_mark(nx);
        if ((nx & 1) == (A387078_prev & 1)) {
            A387078_cur++;
        } else {
            A387078_push_run(A387078_cur);
            A387078_cur = 1;
        }
        A387078_prev = nx;
    }
    return (seq_t)A387078_runs[(size_t)n - 1];
}
