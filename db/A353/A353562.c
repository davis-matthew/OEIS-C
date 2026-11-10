/*
 * A353562  Prime shadows of highly composite numbers, A002182.
 * https://oeis.org/A353562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A181819(A002182(n)). The highly composite numbers are
 *            found by a search: prime by prime, build the Pareto frontier of
 *            numbers with non-increasing exponents (smaller N must have more
 *            divisors), using a streaming merge so no candidate list is
 *            materialised. a(n) = Product prime(e_i) over the...
 * Range:     u8 n <= 29, u16 n <= 121, u32 n <= 433, u64 n <= 1540,
 *            u128 n <= 5699, big any n
 * Validated: standard, pass, 82 terms through n = 9942 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include <stddef.h>

static uint32_t *A353562_pr = NULL;
static size_t A353562_prn = 0;

static void A353562_sieve(size_t need)
{
    size_t lim = 1024;
    if (A353562_prn >= need) return;
    for (;;) {
        char *c = (char *)calloc(lim + 1, 1);
        size_t cnt = 0, i, j;
        for (i = 2; i <= lim; ++i) {
            if (c[i]) continue;
            ++cnt;
            for (j = i * i; j <= lim; j += i) c[j] = 1;
        }
        if (cnt >= need) {
            free(A353562_pr);
            A353562_pr = (uint32_t *)malloc(cnt * sizeof(uint32_t));
            A353562_prn = 0;
            for (i = 2; i <= lim; ++i)
                if (!c[i]) A353562_pr[A353562_prn++] = (uint32_t)i;
            free(c);
            return;
        }
        free(c);
        lim *= 2;
    }
}

static uint32_t A353562_prime(size_t k)
{
    A353562_sieve(k);
    return A353562_pr[k - 1];
}

typedef struct { long double v; int j; int i; } A353562_hn;

static void A353562_hpush(A353562_hn *h, size_t *hs, A353562_hn x)
{
    size_t c = (*hs)++;
    while (c > 0) {
        size_t p = (c - 1) / 2;
        if (h[p].v <= x.v) break;
        h[c] = h[p];
        c = p;
    }
    h[c] = x;
}

static A353562_hn A353562_hpop(A353562_hn *h, size_t *hs)
{
    A353562_hn top = h[0];
    A353562_hn last = h[--(*hs)];
    size_t c = 0, n = *hs;
    for (;;) {
        size_t l = 2 * c + 1, r = l + 1, m = c;
        long double mv = last.v;
        if (l < n && h[l].v < mv) { m = l; mv = h[l].v; }
        if (r < n && h[r].v < mv) { m = r; }
        if (m == c) break;
        h[c] = h[m];
        c = m;
    }
    if (n > 0) h[c] = last;
    return top;
}

static int A353562_find(const int *t, int node, int nl, int nr, int s, int j)
{
    int mid, r;
    if (nr < s || t[node] < j) return -1;
    if (nl == nr) return nl;
    mid = (nl + nr) / 2;
    r = A353562_find(t, 2 * node, nl, mid, s, j);
    if (r >= 0) return r;
    return A353562_find(t, 2 * node + 1, mid + 1, nr, s, j);
}

static int **A353562_par = NULL;
static int **A353562_ex = NULL;
static int A353562_K = 0;
static size_t A353562_count = 0;
static long double A353562_L = 0;

static void A353562_free_stages(void)
{
    int k;
    if (A353562_par) {
        for (k = 1; k <= A353562_K; ++k) {
            free(A353562_par[k]);
            free(A353562_ex[k]);
        }
        free(A353562_par);
        free(A353562_ex);
    }
    A353562_par = NULL;
    A353562_ex = NULL;
    A353562_K = 0;
    A353562_count = 0;
}

static void A353562_build(long double L)
{
    const long double EPS = 1e-12L;
    int K = 0, k, j;
    long double cum = 0;
    int Jcap;
    long double *lg;
    long double *cN, *cD;
    int *cE;
    size_t F;

    A353562_free_stages();
    for (;;) {
        cum += logl((long double)A353562_prime((size_t)K + 1));
        if (cum > L) break;
        ++K;
    }
    if (K == 0) K = 1;
    Jcap = (int)(L / logl(2.0L)) + 3;
    A353562_prime((size_t)Jcap + 2);
    lg = (long double *)malloc((size_t)(Jcap + 2) * sizeof(long double));
    for (j = 0; j < Jcap + 2; ++j) lg[j] = (j == 0) ? 0 : logl((long double)j);

    A353562_K = K;
    A353562_par = (int **)malloc((size_t)(K + 1) * sizeof(int *));
    A353562_ex = (int **)malloc((size_t)(K + 1) * sizeof(int *));
    A353562_par[0] = NULL;
    A353562_ex[0] = NULL;

    F = 1;
    cN = (long double *)malloc(sizeof(long double));
    cD = (long double *)malloc(sizeof(long double));
    cE = (int *)malloc(sizeof(int));
    cN[0] = 0; cD[0] = 0; cE[0] = Jcap;

    for (k = 1; k <= K; ++k) {
        long double lnp = logl((long double)A353562_prime((size_t)k));
        int maxE = 0, J, sz = 1;
        size_t i, hs = 0, m = 0, cap = 1024;
        int *t;
        A353562_hn *h;
        long double *oN, *oD, best = -1.0L;
        int *oP, *oE;

        for (i = 0; i < F; ++i) if (cE[i] > maxE) maxE = cE[i];
        J = (int)(L / lnp);
        if (J > maxE) J = maxE;
        if (J > Jcap) J = Jcap;

        while ((size_t)sz < F) sz *= 2;
        t = (int *)malloc((size_t)(2 * sz) * sizeof(int));
        for (i = 0; i < (size_t)sz; ++i) t[sz + i] = (i < F) ? cE[i] : -1;
        for (i = (size_t)sz - 1; i >= 1; --i)
            t[i] = t[2 * i] > t[2 * i + 1] ? t[2 * i] : t[2 * i + 1];

        h = (A353562_hn *)malloc((size_t)(J + 2) * sizeof(A353562_hn));
        for (j = 0; j <= J; ++j) {
            int s = A353562_find(t, 1, 0, sz - 1, 0, j);
            if (s >= 0 && cN[s] + (long double)j * lnp <= L) {
                A353562_hn x;
                x.v = cN[s] + (long double)j * lnp; x.j = j; x.i = s;
                A353562_hpush(h, &hs, x);
            }
        }

        oN = (long double *)malloc(cap * sizeof(long double));
        oD = (long double *)malloc(cap * sizeof(long double));
        oP = (int *)malloc(cap * sizeof(int));
        oE = (int *)malloc(cap * sizeof(int));

        while (hs > 0) {
            A353562_hn x = A353562_hpop(h, &hs);
            int jj = x.j, ii = x.i, s;
            long double dv = cD[ii] + lg[jj + 1], thr;
            size_t lo, hi;
            if (dv > best + EPS) {
                if (m == cap) {
                    cap *= 2;
                    oN = (long double *)realloc(oN, cap * sizeof(long double));
                    oD = (long double *)realloc(oD, cap * sizeof(long double));
                    oP = (int *)realloc(oP, cap * sizeof(int));
                    oE = (int *)realloc(oE, cap * sizeof(int));
                }
                oN[m] = x.v; oD[m] = dv; oP[m] = ii; oE[m] = jj;
                ++m;
                best = dv;
            }
            /* skip stream elements already dominated: need cD[s] > thr */
            thr = best - lg[jj + 1] + EPS;
            lo = (size_t)ii + 1; hi = F;
            while (lo < hi) {
                size_t mid = lo + (hi - lo) / 2;
                if (cD[mid] > thr) hi = mid; else lo = mid + 1;
            }
            if (lo >= F) continue;
            s = A353562_find(t, 1, 0, sz - 1, (int)lo, jj);
            if (s >= 0 && cN[s] + (long double)jj * lnp <= L) {
                A353562_hn y;
                y.v = cN[s] + (long double)jj * lnp; y.j = jj; y.i = s;
                A353562_hpush(h, &hs, y);
            }
        }
        free(h);
        free(t);
        free(cN); free(cD); free(cE);
        free(oN == NULL ? NULL : (void *)0);
        A353562_par[k] = oP;
        A353562_ex[k] = (int *)malloc((m ? m : 1) * sizeof(int));
        for (i = 0; i < m; ++i) A353562_ex[k][i] = oE[i];
        cN = oN; cD = oD; cE = oE;
        F = m;
    }
    A353562_count = F;
    A353562_L = L;
    free(cN); free(cD); free(cE);
    free(lg);
}

seq_t a(idx_t n)
{
    size_t idx;
    int k;
    seq_t result = (seq_t)1;
    if (n < 1) return (seq_t)0;
    while (A353562_count < (size_t)n + 1) {
        long double L;
        if (A353562_L <= 0) {
            L = 60.0L;
        } else {
            double ratio = (double)(n + 1) / (double)(A353562_count ? A353562_count : 1);
            double f = pow(ratio, 0.8) * 1.05;
            if (f < 1.25) f = 1.25;
            L = A353562_L * (long double)f;
        }
        A353562_build(L);
    }
    idx = (size_t)n - 1;
    for (k = A353562_K; k >= 1; --k) {
        int e = A353562_ex[k][idx];
        if (e > 0) result = result * (seq_t)A353562_prime((size_t)e);
        idx = (size_t)A353562_par[k][idx];
    }
    return result;
}
