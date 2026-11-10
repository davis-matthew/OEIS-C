/*
 * A349316  Where records occur in A348674.
 * https://oeis.org/A349316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search, length by length and in increasing order, for the
 *            numbers where A348674 reaches a new maximum. A depth-first
 *            search runs over the digits. Each prefix keeps the set of states
 *            (sum of finished parts, current part). A prefix is dropped when
 *            an upper bound on the number of distinct sums any c...
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: standard, pass, 52 terms through n = 104 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct { uint64_t s, c; } A349316_st;

static A349316_st *A349316_lvl[21];
static uint64_t *A349316_t0 = 0, *A349316_t1 = 0;
static int A349316_D;
static uint64_t A349316_best, A349316_full;
static int A349316_done;
static uint64_t *A349316_rec = 0;
static size_t A349316_cnt = 0, A349316_cap = 0;
static uint64_t A349316_p10[20];

static int A349316_cmp_st(const void *a, const void *b)
{
    const A349316_st *x = (const A349316_st *)a, *y = (const A349316_st *)b;
    if (x->s != y->s) return x->s < y->s ? -1 : 1;
    if (x->c != y->c) return x->c < y->c ? -1 : 1;
    return 0;
}

static int A349316_cmp_u64(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return (x < y) ? -1 : (x > y);
}

static void A349316_push(uint64_t v)
{
    if (A349316_cnt == A349316_cap) {
        A349316_cap = A349316_cap ? A349316_cap * 2 : 64;
        A349316_rec = (uint64_t *)realloc(A349316_rec, A349316_cap * sizeof(uint64_t));
    }
    A349316_rec[A349316_cnt++] = v;
}

/* sorted unique values s + c*10^j into buf; returns count */
static size_t A349316_vals(const A349316_st *st, size_t m, int j, uint64_t *buf)
{
    uint64_t p = A349316_p10[j];
    for (size_t i = 0; i < m; ++i) buf[i] = st[i].s + st[i].c * p;
    if (m > 1) qsort(buf, m, sizeof(uint64_t), A349316_cmp_u64);
    size_t w = m ? 1 : 0;
    for (size_t i = 1; i < m; ++i)
        if (buf[i] != buf[w - 1]) buf[w++] = buf[i];
    return w;
}

static uint64_t A349316_union(const uint64_t *x, size_t nx, const uint64_t *y, size_t ny)
{
    size_t i = 0, j = 0;
    uint64_t u = 0;
    while (i < nx && j < ny) {
        if (x[i] < y[j]) ++i;
        else if (x[i] > y[j]) ++j;
        else { ++i; ++j; }
        ++u;
    }
    return u + (uint64_t)(nx - i) + (uint64_t)(ny - j);
}

static void A349316_dfs(int k, size_t m, uint64_t val)
{
    int r = A349316_D - k - 1;
    A349316_st *src = A349316_lvl[k];
    A349316_st *dst = A349316_lvl[k + 1];
    for (int x = (k == 0 ? 1 : 0); x <= 9; ++x) {
        if (A349316_done) return;
        size_t mm;
        if (k == 0) {
            dst[0].s = 0; dst[0].c = (uint64_t)x; mm = 1;
        } else {
            for (size_t i = 0; i < m; ++i) {
                dst[2 * i].s = src[i].s;
                dst[2 * i].c = src[i].c * 10 + (uint64_t)x;
                dst[2 * i + 1].s = src[i].s + src[i].c;
                dst[2 * i + 1].c = (uint64_t)x;
            }
            mm = 2 * m;
            qsort(dst, mm, sizeof(A349316_st), A349316_cmp_st);
            size_t w = 1;
            for (size_t i = 1; i < mm; ++i)
                if (dst[i].s != dst[w - 1].s || dst[i].c != dst[w - 1].c)
                    dst[w++] = dst[i];
            mm = w;
        }
        uint64_t nv = val * 10 + (uint64_t)x;
        if (r == 0) {
            uint64_t c = (uint64_t)A349316_vals(dst, mm, 0, A349316_t0);
            if (c > A349316_best) {
                A349316_best = c;
                A349316_push(nv);
                if (A349316_best >= A349316_full) A349316_done = 1;
            }
            continue;
        }
        if (((uint64_t)mm << r) <= A349316_best) continue;
        size_t u0 = A349316_vals(dst, mm, 0, A349316_t0);
        uint64_t B = 0;
        for (int j = 1; j <= r; ++j) {
            size_t uj = A349316_vals(dst, mm, j, A349316_t1);
            uint64_t un = A349316_union(A349316_t0, u0, A349316_t1, uj);
            B += (j < r) ? (un << (r - j - 1)) : un;
        }
        if (B > A349316_best) A349316_dfs(k + 1, mm, nv);
    }
}

static void A349316_search_length(int d)
{
    if (!A349316_lvl[0]) A349316_lvl[0] = (A349316_st *)malloc(sizeof(A349316_st));
    for (int k = 1; k <= d; ++k)
        if (!A349316_lvl[k])
            A349316_lvl[k] = (A349316_st *)malloc(((size_t)1 << k) * sizeof(A349316_st));
    A349316_t0 = (uint64_t *)realloc(A349316_t0, ((size_t)1 << d) * sizeof(uint64_t));
    A349316_t1 = (uint64_t *)realloc(A349316_t1, ((size_t)1 << d) * sizeof(uint64_t));
    A349316_D = d;
    A349316_full = (uint64_t)1 << (d - 1);
    A349316_done = 0;
    A349316_dfs(0, 0, 0);
}

seq_t a(idx_t n)
{
    static int init = 0, next_d = 2;
    if (!init) {
        A349316_p10[0] = 1;
        for (int i = 1; i < 20; ++i) A349316_p10[i] = A349316_p10[i - 1] * 10;
        A349316_push(0);       /* A348674(0) = 1 */
        A349316_best = 1;      /* single digits 1..9 also give 1 */
        init = 1;
    }
    while (A349316_cnt < (size_t)n && next_d <= 19) {
        A349316_search_length(next_d);
        ++next_d;
    }
    if ((size_t)n > A349316_cnt || n < 1) return (seq_t)0;
    return (seq_t)A349316_rec[n - 1];
}
