/*
 * A330688  Record values in A050377, number of ways to factor n into
 *          "Fermi-Dirac primes" (A050376).
 * https://oeis.org/A330688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A050377(A330687(n)), where A050377 is multiplicative with
 *            A050377(p^e) = f[e], f[0] = 1, f[e] = Sum_{k=0..floor(e/2)}
 *            f[k]. Computed by a search: since f[2k+1] = f[2k] and f[1] = 1,
 *            every record position is a square whose exponents do not
 *            increase. Its largest prime p_k therefore satisfies...
 * Range:     u8 n <= 29, u16 n <= 121, u32 n <= 544, u64 n <= 1004,
 *            u128 n <= 1004, big any n
 * Validated: standard, pass, 84 terms through n = 1003 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/mem.h"

static seq_t *A330688_f = NULL;
static size_t A330688_fn = 0;

static long double A330688_plog[400];
static int A330688_np = 0;

static seq_t *A330688_rec = NULL;
static size_t A330688_nrec = 0;
static long double A330688_L = 0;

static const long double *A330688_sortkey = NULL;

static void A330688_ensure_f(size_t maxe)
{
    if (maxe < A330688_fn) return;
    size_t nn = maxe + 1;
    if (A330688_f) mem_free(A330688_f);
    A330688_f = mem_new(seq_t, nn);
    A330688_f[0] = 1;
    seq_t pref = 1;
    size_t upto = 0;
    for (size_t e = 1; e < nn; ++e) {
        size_t lim = e >> 1;
        while (upto < lim) { ++upto; pref += A330688_f[upto]; }
        A330688_f[e] = pref;
    }
    A330688_fn = nn;
}

static void A330688_ensure_primes(int cnt)
{
    if (A330688_np >= cnt) return;
    long c = 2;
    int k = 0;
    while (k < cnt) {
        bool pr = true;
        for (long d = 2; d * d <= c; ++d) if (c % d == 0) { pr = false; break; }
        if (pr) A330688_plog[k++] = logl((long double)c);
        ++c;
    }
    A330688_np = cnt;
}

static int A330688_cmp(const void *x, const void *y)
{
    size_t i = *(const size_t *)x, j = *(const size_t *)y;
    if (A330688_sortkey[i] < A330688_sortkey[j]) return -1;
    if (A330688_sortkey[i] > A330688_sortkey[j]) return 1;
    return (i < j) ? -1 : (i > j ? 1 : 0);
}

static void A330688_build(long double L)
{
    A330688_ensure_primes(400);
    A330688_ensure_f((size_t)(L / A330688_plog[0]) + 4);

    size_t cn = 1;
    long double *cl = (long double *)malloc(sizeof(long double));
    seq_t *cv = mem_new(seq_t, 1);
    cl[0] = 0.0L;
    cv[0] = 1;

    long double sumlog = 0.0L;
    for (int i = 0; i < 400; ++i) {
        long double lp = A330688_plog[i];
        sumlog += lp;
        if (2.0L * sumlog > L + 1e-9L) break;

        size_t tot = 0;
        for (size_t k = 0; k < cn; ++k) {
            long double room = L - cl[k];
            size_t m = 1;
            if (room >= 2.0L * lp - 1e-12L) m += (size_t)((room + 1e-12L) / (2.0L * lp));
            tot += m;
        }
        long double *nl = (long double *)malloc(tot * sizeof(long double));
        seq_t *nv = mem_new(seq_t, tot);
        size_t t = 0;
        for (size_t k = 0; k < cn; ++k) {
            size_t e = 0;
            long double l = cl[k];
            while (l <= L + 1e-12L) {
                nl[t] = l;
                nv[t] = cv[k] * A330688_f[e];
                ++t;
                e += 2;
                l += 2.0L * lp;
                if (t >= tot) break;
            }
        }
        size_t *ord = (size_t *)malloc(t * sizeof(size_t));
        for (size_t k = 0; k < t; ++k) ord[k] = k;
        A330688_sortkey = nl;
        qsort(ord, t, sizeof(size_t), A330688_cmp);

        size_t keep = 0;
        bool have = false;
        seq_t best = 0;
        for (size_t k = 0; k < t; ++k) {
            if (!have || nv[ord[k]] > best) {
                have = true;
                best = nv[ord[k]];
                ++keep;
            }
        }
        long double *kl = (long double *)malloc((keep ? keep : 1) * sizeof(long double));
        seq_t *kv = mem_new(seq_t, keep ? keep : 1);
        size_t q = 0;
        have = false;
        for (size_t k = 0; k < t; ++k) {
            if (!have || nv[ord[k]] > best) {
                have = true;
                best = nv[ord[k]];
                kl[q] = nl[ord[k]];
                kv[q] = nv[ord[k]];
                ++q;
            }
        }
        free(ord);
        free(nl);
        mem_free(nv);
        free(cl);
        mem_free(cv);
        cl = kl;
        cv = kv;
        cn = keep;
    }

    free(cl);
    if (A330688_rec) mem_free(A330688_rec);
    A330688_rec = cv;
    A330688_nrec = cn;
    A330688_L = L;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A330688_nrec < (size_t)n) {
        long double L = A330688_L > 0 ? A330688_L * 1.5L : 64.0L;
        for (;;) {
            A330688_build(L);
            if (A330688_nrec >= (size_t)n) break;
            L *= 1.5L;
        }
    }
    return A330688_rec[(size_t)n - 1];
}
