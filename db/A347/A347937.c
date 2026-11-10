/*
 * A347937  Numbers k such that k and k+1 are both terms of A347935.
 * https://oeis.org/A347937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. Of any two consecutive numbers k, k+1, one is odd, so
 *            that odd number m must itself be in A347935, i.e. A187795(m) >
 *            2*m. A DFS over the prime factorizations of odd m <= N finds
 *            every such m. It prunes with an upper bound on A187795(m)/m that
 *            holds for every extension of the current prefi...
 * Range:     u8 none, u16 none, u32 n <= 1135, u64 n <= 2372, u128 n <= 2372,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 2177 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#define S347937_MAXDEP 64
#define S347937_EPS 1e-9

typedef struct { uint64_t d, s; } s347937_dv;
typedef struct { double h, f; } s347937_hf;

static uint32_t *s347937_P = NULL;
static size_t s347937_NP = 0;
static uint64_t s347937_PL = 0;

static void s347937_extend(void)
{
    uint64_t lim = s347937_PL ? s347937_PL * 2 : 65536;
    unsigned char *c = (unsigned char *)calloc((size_t)lim + 1, 1);
    if (!c) abort();
    size_t cnt = 0;
    for (uint64_t i = 2; i <= lim; ++i) {
        if (!c[i]) {
            cnt++;
            for (uint64_t j = i * i; j <= lim; j += i) c[j] = 1;
        }
    }
    uint32_t *np = (uint32_t *)realloc(s347937_P, (cnt + 1) * sizeof(uint32_t));
    if (!np) abort();
    s347937_P = np;
    size_t k = 0;
    for (uint64_t i = 2; i <= lim; ++i) if (!c[i]) s347937_P[k++] = (uint32_t)i;
    free(c);
    s347937_NP = cnt;
    s347937_PL = lim;
}

static uint64_t s347937_prime(size_t j)
{
    while (j >= s347937_NP) s347937_extend();
    return s347937_P[j];
}

/* upper bound on sigma(w)/w for w <= X with all prime factors >= prime(j) */
static double s347937_R(size_t j, uint64_t X)
{
    double r = 1.0;
    uint64_t prod = 1;
    for (size_t t = j;; ++t) {
        uint64_t p = s347937_prime(t);
        if (p > X / prod) break;
        prod *= p;
        r *= (double)p / (double)(p - 1);
    }
    return r;
}

static void s347937_ensure_dv(s347937_dv **b, size_t *cap, size_t need)
{
    if (need <= *cap) return;
    size_t nc = *cap ? *cap : 64;
    while (nc < need) nc *= 2;
    s347937_dv *t = (s347937_dv *)realloc(*b, nc * sizeof(s347937_dv));
    if (!t) abort();
    *b = t;
    *cap = nc;
}

static size_t s347937_build(const uint64_t *pr, const int *ex, int c,
                            s347937_dv **buf, size_t *cap)
{
    s347937_ensure_dv(buf, cap, 1);
    (*buf)[0].d = 1; (*buf)[0].s = 1;
    size_t nd = 1;
    for (int i = 0; i < c; ++i) {
        size_t base = nd;
        uint64_t pe = 1, sp = 1;
        for (int e = 1; e <= ex[i]; ++e) {
            pe *= pr[i];
            sp += pe;
            s347937_ensure_dv(buf, cap, nd + base);
            s347937_dv *B = *buf;
            for (size_t k = 0; k < base; ++k) {
                B[nd].d = B[k].d * pe;
                B[nd].s = B[k].s * sp;
                nd++;
            }
        }
    }
    return nd;
}

static s347937_dv *s347937_tb = NULL;
static size_t s347937_tbcap = 0;

/* is A187795(x) > 2x ? */
static int s347937_isterm(uint64_t x)
{
    uint64_t pr[64];
    int ex[64];
    int c = 0;
    uint64_t r = x;
    int checked = 0;
    for (size_t t = 0;; ++t) {
        uint64_t p = s347937_prime(t);
        if (p * p > r) break;
        if (r % p == 0) {
            int e = 0;
            while (r % p == 0) { r /= p; e++; }
            pr[c] = p; ex[c] = e; c++;
        }
        if (!checked && p >= 200) {
            checked = 1;
            if (r > 1) {
                size_t nd = s347937_build(pr, ex, c, &s347937_tb, &s347937_tbcap);
                uint64_t s = x / r;
                double R = s347937_R(t + 1, r);
                double thr = 2.0 / R * (1.0 - S347937_EPS);
                double sum = 0.0;
                for (size_t k = 0; k < nd; ++k) {
                    double h = (double)s347937_tb[k].s / (double)s347937_tb[k].d;
                    if (h > thr) sum += (double)s347937_tb[k].d / (double)s;
                }
                if (sum * R < 2.0 * (1.0 - S347937_EPS)) return 0;
            }
        }
    }
    if (r > 1) { pr[c] = r; ex[c] = 1; c++; }
    size_t nd = s347937_build(pr, ex, c, &s347937_tb, &s347937_tbcap);
    uint64_t sum = 0;
    for (size_t k = 0; k < nd; ++k)
        if (s347937_tb[k].s > 2 * s347937_tb[k].d) sum += s347937_tb[k].d;
    return sum > 2 * x;
}

static s347937_dv *s347937_lv[S347937_MAXDEP];
static size_t s347937_lvcap[S347937_MAXDEP];
static s347937_hf *s347937_hv[S347937_MAXDEP];
static size_t s347937_hvcap[S347937_MAXDEP];

static uint64_t s347937_N = 0;
static uint64_t *s347937_found = NULL;
static size_t s347937_fcnt = 0, s347937_fcap = 0;

static void s347937_add(uint64_t k)
{
    if (k >= s347937_N) return;
    if (s347937_fcnt >= s347937_fcap) {
        s347937_fcap = s347937_fcap ? s347937_fcap * 2 : 256;
        uint64_t *t = (uint64_t *)realloc(s347937_found, s347937_fcap * sizeof(uint64_t));
        if (!t) abort();
        s347937_found = t;
    }
    s347937_found[s347937_fcnt++] = k;
}

static int s347937_cmphf(const void *a, const void *b)
{
    double x = ((const s347937_hf *)a)->h, y = ((const s347937_hf *)b)->h;
    return x < y ? 1 : (x > y ? -1 : 0);
}

static int s347937_cmpu64(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a, y = *(const uint64_t *)b;
    return x < y ? -1 : (x > y ? 1 : 0);
}

static void s347937_dfs(int dep, size_t nd, uint64_t v, size_t idx)
{
    s347937_dv *D = s347937_lv[dep];
    if (v > 1) {
        uint64_t sab = 0;
        for (size_t i = 0; i < nd; ++i)
            if (D[i].s > 2 * D[i].d) sab += D[i].d;
        if (sab > 2 * v) {
            if (s347937_isterm(v - 1)) s347937_add(v - 1);
            if (s347937_isterm(v + 1)) s347937_add(v);
        }
    }
    uint64_t X = s347937_N / v;
    if (s347937_prime(idx) > X) return;
    if (dep + 1 >= S347937_MAXDEP) return;

    if (nd > s347937_hvcap[dep]) {
        size_t nc = s347937_hvcap[dep] ? s347937_hvcap[dep] : 64;
        while (nc < nd) nc *= 2;
        s347937_hf *t = (s347937_hf *)realloc(s347937_hv[dep], nc * sizeof(s347937_hf));
        if (!t) abort();
        s347937_hv[dep] = t;
        s347937_hvcap[dep] = nc;
    }
    s347937_hf *H = s347937_hv[dep];
    for (size_t i = 0; i < nd; ++i) {
        H[i].h = (double)D[i].s / (double)D[i].d;
        H[i].f = (double)D[i].d / (double)v;
    }
    qsort(H, nd, sizeof(s347937_hf), s347937_cmphf);
    double acc = 0.0;
    for (size_t i = 0; i < nd; ++i) { acc += H[i].f; H[i].f = acc; }

    size_t ptr = nd;
    for (size_t j = idx;; ++j) {
        uint64_t q = s347937_prime(j);
        if (q > X) break;
        double R = s347937_R(j, X);
        double thr = 2.0 / R * (1.0 - S347937_EPS);
        while (ptr > 0 && H[ptr - 1].h <= thr) --ptr;
        double ub = ptr ? H[ptr - 1].f * R : 0.0;
        if (ub < 2.0 * (1.0 - S347937_EPS)) break;

        int cd = dep + 1;
        s347937_ensure_dv(&s347937_lv[cd], &s347937_lvcap[cd], nd);
        for (size_t i = 0; i < nd; ++i) s347937_lv[cd][i] = D[i];
        size_t cnd = nd;
        uint64_t pe = 1, sp = 1;
        while (pe <= X / q) {
            pe *= q;
            sp += pe;
            s347937_ensure_dv(&s347937_lv[cd], &s347937_lvcap[cd], cnd + nd);
            s347937_dv *C = s347937_lv[cd];
            for (size_t i = 0; i < nd; ++i) {
                C[cnd + i].d = D[i].d * pe;
                C[cnd + i].s = D[i].s * sp;
            }
            cnd += nd;
            s347937_dfs(cd, cnd, v * pe, j + 1);
        }
    }
}

static void s347937_run(uint64_t N)
{
    s347937_N = N;
    s347937_fcnt = 0;
    s347937_ensure_dv(&s347937_lv[0], &s347937_lvcap[0], 1);
    s347937_lv[0][0].d = 1;
    s347937_lv[0][0].s = 1;
    s347937_dfs(0, 1, 1, 1); /* prime(1) = 3: odd numbers only */
    qsort(s347937_found, s347937_fcnt, sizeof(uint64_t), s347937_cmpu64);
    size_t w = 0;
    for (size_t i = 0; i < s347937_fcnt; ++i)
        if (w == 0 || s347937_found[w - 1] != s347937_found[i])
            s347937_found[w++] = s347937_found[i];
    s347937_fcnt = w;
}

seq_t a(idx_t n)
{
    static uint64_t curN = 0;
    if (n == 0) return (seq_t)0;
    while (curN == 0 || s347937_fcnt < n) {
        curN = curN ? curN * 2 : ((uint64_t)1 << 22);
        s347937_run(curN);
    }
    return (seq_t)s347937_found[n - 1];
}
