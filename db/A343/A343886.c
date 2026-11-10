/*
 * A343886  a(n) = k/A097621(k) for k=A344826(n).
 * https://oeis.org/A343886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k/A097621(k) for k = A344826(n). The code finds every k
 *            <= B by a depth-first search over k's prime factorization,
 *            taking primes in increasing order. At each step it keeps only
 *            the choices where every prime factor of A097621(k) = prod
 *            idx(p^e) is covered by k. It doubles B by 16 until it h...
 * Range:     u8 n <= 187, u16 n <= 187, u32 n <= 187, u64 n <= 187,
 *            u128 n <= 187, big any n
 * Validated: standard, pass, 95 terms through n = 182 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t A343886_L = 0, A343886_B = 0;
static uint8_t *A343886_cmp = NULL;
static uint64_t *A343886_pp = NULL;  static size_t A343886_ppc = 0;
static uint64_t *A343886_pr = NULL;  static size_t A343886_prc = 0;
typedef struct { uint64_t k, c; } A343886_rec;
static A343886_rec *A343886_res = NULL;
static size_t A343886_rn = 0, A343886_rcap = 0;

static bool A343886_isprime(uint64_t x)
{
    if (x < 2) return false;
    if (x == 2) return true;
    if (!(x & 1)) return false;
    { uint64_t i = x >> 1; return !((A343886_cmp[i >> 3] >> (i & 7)) & 1); }
}

static int A343886_cmpu(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t*)a, y = *(const uint64_t*)b;
    return (x > y) - (x < y);
}
static int A343886_cmprec(const void *a, const void *b)
{
    uint64_t x = ((const A343886_rec*)a)->k, y = ((const A343886_rec*)b)->k;
    return (x > y) - (x < y);
}

static void A343886_build(uint64_t L)
{
    uint64_t i, j, x, s;
    size_t nb = (size_t)((L >> 4) + 2), hpc = 0, hpcap = 1024, hj, cnt;
    uint64_t *hp;
    free(A343886_cmp); free(A343886_pp); free(A343886_pr);
    A343886_L = L;
    A343886_cmp = (uint8_t*)calloc(nb, 1);
    for (i = 3; i * i <= L; i += 2)
        if (A343886_isprime(i))
            for (j = i * i; j <= L; j += 2 * i) {
                uint64_t b = j >> 1;
                A343886_cmp[b >> 3] |= (uint8_t)(1u << (b & 7));
            }
    s = 1; while ((s + 1) * (s + 1) <= L) s++;
    A343886_prc = 0;
    A343886_pr = (uint64_t*)malloc(sizeof(uint64_t) * (size_t)(s + 2));
    for (i = 2; i <= s + 1; i++) if (A343886_isprime(i)) A343886_pr[A343886_prc++] = i;
    hp = (uint64_t*)malloc(sizeof(uint64_t) * hpcap);
    for (j = 0; j < A343886_prc; j++) {
        uint64_t p = A343886_pr[j], v;
        if (p > L / p) break;
        v = p * p;
        while (v <= L) {
            if (hpc == hpcap) { hpcap *= 2; hp = (uint64_t*)realloc(hp, sizeof(uint64_t) * hpcap); }
            hp[hpc++] = v;
            if (v > L / p) break;
            v *= p;
        }
    }
    qsort(hp, hpc, sizeof(uint64_t), A343886_cmpu);
    cnt = 1 + hpc;
    for (x = 2; x <= L; x++) if (A343886_isprime(x)) cnt++;
    A343886_pp = (uint64_t*)malloc(sizeof(uint64_t) * cnt);
    A343886_ppc = 0; hj = 0;
    A343886_pp[A343886_ppc++] = 1;
    for (x = 2; x <= L; x++) {
        if (A343886_isprime(x)) A343886_pp[A343886_ppc++] = x;
        else if (hj < hpc && hp[hj] == x) { A343886_pp[A343886_ppc++] = x; hj++; }
    }
    free(hp);
}

/* number of prime powers (incl. 1) <= x; equals index of x if x is one */
static uint64_t A343886_cntle(uint64_t x)
{
    size_t lo = 0, hi = A343886_ppc;
    while (lo < hi) { size_t m = (lo + hi) / 2; if (A343886_pp[m] <= x) lo = m + 1; else hi = m; }
    return (uint64_t)lo;
}

static int A343886_factor(uint64_t x, uint64_t *fs, int *fv)
{
    int m = 0; size_t i;
    for (i = 0; i < A343886_prc && A343886_pr[i] * A343886_pr[i] <= x; i++) {
        uint64_t p = A343886_pr[i];
        if (x % p == 0) { int v = 0; while (x % p == 0) { x /= p; v++; } fs[m] = p; fv[m] = v; m++; }
    }
    if (x > 1) { fs[m] = x; fv[m] = 1; m++; }
    return m;
}

typedef struct {
    uint64_t dp[48]; int de[48], df[48]; int dc;
    uint64_t rp[48]; int rr[48]; int rc;
    uint64_t N, F, last;
} A343886_St;

static int A343886_apply(const A343886_St *s, A343886_St *t, uint64_t p, int e, uint64_t q, uint64_t iv)
{
    uint64_t fs[64]; int fv[64], m, i, j, k, req = 0;
    *t = *s;
    t->N *= q; t->F *= iv; t->last = p;
    for (i = 0; i < t->rc; i++) if (t->rp[i] == p) {
        req = t->rr[i]; t->rc--; t->rp[i] = t->rp[t->rc]; t->rr[i] = t->rr[t->rc]; break;
    }
    k = t->dc++;
    t->dp[k] = p; t->de[k] = e; t->df[k] = req;
    m = A343886_factor(iv, fs, fv);
    for (i = 0; i < m; i++) {
        uint64_t sp = fs[i];
        if (sp < p) {
            for (j = 0; j < k; j++) if (t->dp[j] == sp) break;
            if (j == k) return 0;
            t->df[j] += fv[i];
            if (t->df[j] > t->de[j]) return 0;
        } else if (sp == p) {
            t->df[k] += fv[i];
        } else {
            for (j = 0; j < t->rc; j++) if (t->rp[j] == sp) break;
            if (j == t->rc) { if (t->rc >= 48) return 0; t->rp[j] = sp; t->rr[j] = 0; t->rc++; }
            t->rr[j] += fv[i];
        }
    }
    if (t->df[k] > t->de[k]) return 0;
    return 1;
}

static void A343886_dfs(const A343886_St *s);

/* divisor enumeration for the e = 1, p > 5 case */
static void A343886_divs(const A343886_St *s, const uint64_t *sp, const int *ex, int m, int i,
                         uint64_t d, uint64_t dmax, uint64_t rmin, uint64_t lim)
{
    int j;
    if (i == m) {
        uint64_t q;
        A343886_St t;
        if (d < 1 || d > A343886_ppc) return;
        q = A343886_pp[d - 1];
        if (q > 5 && q > s->last && q < rmin && q <= lim && A343886_isprime(q))
            if (A343886_apply(s, &t, q, 1, q, d)) A343886_dfs(&t);
        return;
    }
    for (j = 0; j <= ex[i]; j++) {
        A343886_divs(s, sp, ex, m, i + 1, d, dmax, rmin, lim);
        if (d > dmax / sp[i]) break;
        d *= sp[i];
    }
}

static void A343886_dfs(const A343886_St *s)
{
    uint64_t lim = A343886_B / s->N, rmin = UINT64_MAX, cap, need = 1;
    int i, rreq = 0;
    A343886_St t;
    for (i = 0; i < s->rc; i++) {
        int v;
        for (v = 0; v < s->rr[i]; v++) { if (need > lim / s->rp[i]) return; need *= s->rp[i]; }
        if (s->rp[i] < rmin) { rmin = s->rp[i]; rreq = s->rr[i]; }
    }
    (void)rreq;
    if (s->rc == 0) {
        if (A343886_rn == A343886_rcap) {
            A343886_rcap = A343886_rcap ? 2 * A343886_rcap : 1024;
            A343886_res = (A343886_rec*)realloc(A343886_res, sizeof(A343886_rec) * A343886_rcap);
        }
        A343886_res[A343886_rn].k = s->N;
        A343886_res[A343886_rn].c = s->N / s->F;
        A343886_rn++;
    }
    cap = lim < A343886_L ? lim : A343886_L;
    /* (c) the smallest required prime */
    if (s->rc > 0) {
        uint64_t q = 1; int e;
        for (e = 1; ; e++) {
            if (q > cap / rmin) break;
            q *= rmin;
            if (A343886_apply(s, &t, rmin, e, q, A343886_cntle(q))) A343886_dfs(&t);
        }
    }
    /* (a) primes p > 5 with exponent 1: idx(p) must divide the surplus */
    {
        uint64_t sp[48]; int ex[48], m = 0;
        uint64_t dmax = A343886_cntle(cap);
        for (i = 0; i < s->dc; i++) if (s->de[i] > s->df[i]) { sp[m] = s->dp[i]; ex[m] = s->de[i] - s->df[i]; m++; }
        if (dmax >= 4) A343886_divs(s, sp, ex, m, 0, 1, dmax, rmin, lim);
    }
    /* (b) small primes: p <= 5 any exponent, others exponent >= 2 */
    {
        size_t j;
        for (j = 0; j < A343886_prc; j++) {
            uint64_t p = A343886_pr[j], q;
            int e;
            if (p <= s->last) continue;
            if (p >= rmin) break;
            if (p > 5 && p > cap / p) break;
            if (p > cap) break;
            q = 1;
            for (e = 1; ; e++) {
                if (q > cap / p) break;
                q *= p;
                if (e == 1 && p > 5) continue;
                if (A343886_apply(s, &t, p, e, q, A343886_cntle(q))) A343886_dfs(&t);
            }
        }
    }
}

seq_t a(idx_t n)
{
    while ((uint64_t)n > A343886_rn || A343886_B == 0) {
        uint64_t B = A343886_B ? A343886_B * 16 : ((uint64_t)1 << 22);
        uint64_t r = 1, L;
        A343886_St root;
        while ((r + 1) * (r + 1) <= B) r++;
        L = 32 * r;
        if (L < ((uint64_t)1 << 16)) L = (uint64_t)1 << 16;
        A343886_B = B;
        A343886_build(L);
        A343886_rn = 0;
        root.dc = 0; root.rc = 0; root.N = 1; root.F = 1; root.last = 1;
        A343886_dfs(&root);
        qsort(A343886_res, A343886_rn, sizeof(A343886_rec), A343886_cmprec);
        if (B >= ((uint64_t)1 << 58)) break;
    }
    if ((uint64_t)n > A343886_rn) return (seq_t)0;
    return (seq_t)A343886_res[n - 1].c;
}
