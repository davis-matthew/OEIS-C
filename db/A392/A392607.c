/*
 * A392607  Squarefree numbers k such that A322582(k) <= A276085(k) <=
 *          A348507(k).
 * https://oeis.org/A392607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th squarefree k with A322582(k) <= A276085(k) <=
 *            A348507(k), i.e. k-phi(k) <= A276085(k) <= psi(k)-k (psi =
 *            A001615-type, prod (p+1)). Method: enumerate squarefree k by
 *            largest prime P (subset DFS over smaller primes), sorted, with a
 *            lower bound on k for larger P
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

typedef unsigned __int128 a392607_u128;

static const unsigned a392607_pr[25] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
static a392607_u128 a392607_pre[26]; /* product of primes before index i */
static a392607_u128 *a392607_terms = NULL;
static size_t a392607_cnt = 0, a392607_cap = 0;
static int a392607_done = 0;  /* number of primes processed */
static int a392607_init = 0;

static void a392607_push(a392607_u128 v)
{
    if (a392607_cnt == a392607_cap) {
        size_t nc = a392607_cap ? a392607_cap * 2 : 256;
        a392607_u128 *np = (a392607_u128 *)realloc(a392607_terms, nc * sizeof(a392607_u128));
        if (!np) return;
        a392607_terms = np;
        a392607_cap = nc;
    }
    a392607_terms[a392607_cnt++] = v;
}

static void a392607_rec(int i, int lim, a392607_u128 k, a392607_u128 phi,
                        a392607_u128 psi, a392607_u128 S, unsigned P)
{
    if (i == lim) {
        a392607_u128 K = k * P, PH = phi * (P - 1), PS = psi * (P + 1);
        a392607_u128 SS = S + a392607_pre[lim];
        if (K - PH <= SS && SS <= PS - K)
            a392607_push(K);
        return;
    }
    a392607_rec(i + 1, lim, k, phi, psi, S, P);
    a392607_rec(i + 1, lim, k * a392607_pr[i], phi * (a392607_pr[i] - 1),
                psi * (a392607_pr[i] + 1), S + a392607_pre[i], P);
}

static int a392607_cmp(const void *x, const void *y)
{
    a392607_u128 a = *(const a392607_u128 *)x, b = *(const a392607_u128 *)y;
    return a < b ? -1 : (a > b ? 1 : 0);
}

/* number of terms guaranteed final (all terms below lower bound for larger primes) */
static size_t a392607_stable(void)
{
    if (a392607_done >= 25) return a392607_cnt;
    {
        int j = a392607_done;           /* next prime index */
        a392607_u128 Q = a392607_pre[j];            /* product of primes before next */
        a392607_u128 Q2 = Q * a392607_pr[j];        /* includes next prime */
        a392607_u128 ps = 1;
        int t;
        double L;
        for (t = 0; t <= j; t++) ps *= (a392607_pr[t] + 1);
        {
            double d = (double)(ps - Q2);
            L = (double)Q * ((double)Q2 / d) * 0.99;
        }
        {
            size_t c = 0;
            while (c < a392607_cnt && (double)a392607_terms[c] < L) c++;
            return c;
        }
    }
}

static seq_t a392607_conv(a392607_u128 x)
{
    seq_t r = (seq_t)(uint64_t)(x >> 64);
    r = r * (seq_t)65536; r = r * (seq_t)65536;
    r = r * (seq_t)65536; r = r * (seq_t)65536;
    r += (seq_t)(uint64_t)x;
    return r;
}

seq_t a(idx_t n)
{
    if (!a392607_init) {
        int i;
        a392607_init = 1;
        a392607_pre[0] = 1;
        for (i = 0; i < 25; i++) a392607_pre[i + 1] = a392607_pre[i] * a392607_pr[i];
        a392607_push(1);
    }
    while (a392607_stable() < (size_t)n && a392607_done < 25) {
        a392607_rec(0, a392607_done, 1, 1, 1, 0, a392607_pr[a392607_done]);
        a392607_done++;
        qsort(a392607_terms, a392607_cnt, sizeof(a392607_u128), a392607_cmp);
    }
    if (n < 1 || (size_t)n > a392607_cnt) return 0;
    return a392607_conv(a392607_terms[n - 1]);
}
