/*
 * A324581  a(n) = A276086(A002182(n)).
 * https://oeis.org/A324581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A276086(A002182(n)); A002182(n) is the n-th highly
 *            composite number, found by a Pareto-frontier search over
 *            exponent vectors; A276086 maps its primorial base digits to
 *            exponents of successive primes
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 669 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

#define HC_NP 96

typedef struct {
    long double lv;
    unsigned __int128 d;
    int f;
    unsigned char ex[HC_NP];
} HcSt;

static int hc_cmp_f_lv(const void *a, const void *b)
{
    const HcSt *x = (const HcSt *)a, *y = (const HcSt *)b;
    if (x->f != y->f) return x->f < y->f ? -1 : 1;
    if (x->lv != y->lv) return x->lv < y->lv ? -1 : 1;
    return 0;
}

static int hc_cmp_lv(const void *a, const void *b)
{
    const HcSt *x = (const HcSt *)a, *y = (const HcSt *)b;
    if (x->lv != y->lv) return x->lv < y->lv ? -1 : 1;
    return 0;
}

static unsigned hc_primes[HC_NP];
static long double hc_lp[HC_NP];

static void hc_init_primes(void)
{
    static int done = 0;
    if (done) return;
    int c = 0;
    for (unsigned p = 2; c < HC_NP; ++p) {
        bool ok = true;
        for (int i = 0; i < c; ++i) {
            if (hc_primes[i] * hc_primes[i] > p) break;
            if (p % hc_primes[i] == 0) { ok = false; break; }
        }
        if (ok) hc_primes[c++] = p;
    }
    for (int i = 0; i < HC_NP; ++i) {
        /* log via repeated series-free method: use Newton on exp is heavy; use atanh series */
        long double x = (long double)hc_primes[i];
        int k = 0;
        while (x > 1.5L) { x /= 2.0L; ++k; }
        long double t = (x - 1.0L) / (x + 1.0L), t2 = t * t, s = 0, pw = t;
        for (int j = 1; j < 60; j += 2) { s += pw / j; pw *= t2; }
        long double ln2;
        {
            long double u = 1.0L / 3.0L, u2 = u * u, ss = 0, pp = u;
            for (int j = 1; j < 80; j += 2) { ss += pp / j; pp *= u2; }
            ln2 = 2.0L * ss;
        }
        hc_lp[i] = 2.0L * s + k * ln2;
    }
    done = 1;
}

static HcSt *hc_list = NULL;
static size_t hc_count = 0;

static void hc_build(long double B)
{
    size_t ccap = 1024, ccnt = 0;
    HcSt *cand = (HcSt *)malloc(ccap * sizeof(HcSt));
    size_t ucap = 1024, ucnt = 1;
    HcSt *cur = (HcSt *)malloc(ucap * sizeof(HcSt));
    memset(&cur[0], 0, sizeof(HcSt));
    cur[0].d = 1;
    cur[0].f = 200;
    cand[ccnt++] = cur[0];
    for (int k = 0; k < HC_NP; ++k) {
        size_t ncap = 1024, ncnt = 0;
        HcSt *nx = (HcSt *)malloc(ncap * sizeof(HcSt));
        for (size_t i = 0; i < ucnt; ++i) {
            long double lv = cur[i].lv;
            for (int f = 1; f <= cur[i].f; ++f) {
                lv += hc_lp[k];
                if (lv > B) break;
                if (ncnt == ncap) {
                    ncap *= 2;
                    nx = (HcSt *)realloc(nx, ncap * sizeof(HcSt));
                }
                HcSt *c = &nx[ncnt++];
                *c = cur[i];
                c->lv = lv;
                c->ex[k] = (unsigned char)f;
                c->d = cur[i].d * (unsigned)(f + 1);
                c->f = f;
            }
        }
        if (ncnt == 0) { free(nx); break; }
        qsort(nx, ncnt, sizeof(HcSt), hc_cmp_f_lv);
        size_t keep = 0;
        int gf = -1;
        unsigned __int128 maxd = 0;
        for (size_t i = 0; i < ncnt; ++i) {
            if (nx[i].f != gf) { gf = nx[i].f; maxd = 0; }
            if (nx[i].d > maxd) {
                maxd = nx[i].d;
                nx[keep++] = nx[i];
            }
        }
        free(cur);
        cur = nx;
        ucnt = keep;
        ucap = ncap;
        for (size_t i = 0; i < ucnt; ++i) {
            if (ccnt == ccap) {
                ccap *= 2;
                cand = (HcSt *)realloc(cand, ccap * sizeof(HcSt));
            }
            cand[ccnt++] = cur[i];
        }
    }
    free(cur);
    (void)ucap;
    qsort(cand, ccnt, sizeof(HcSt), hc_cmp_lv);
    free(hc_list);
    hc_list = (HcSt *)malloc(ccnt * sizeof(HcSt));
    hc_count = 0;
    unsigned __int128 best = 0;
    for (size_t i = 0; i < ccnt; ++i) {
        if (cand[i].d > best) {
            best = cand[i].d;
            hc_list[hc_count++] = cand[i];
        }
    }
    free(cand);
}

seq_t a(idx_t n)
{
    hc_init_primes();
    static long double B = 20.0L;
    if (hc_count == 0) hc_build(B);
    while (hc_count < (size_t)n) {
        B *= 1.4L;
        hc_build(B);
    }
    const HcSt *h = &hc_list[(size_t)n - 1];
    seq_t N = 1;
    for (int k = 0; k < HC_NP; ++k) {
        for (int e = 0; e < h->ex[k]; ++e) N *= (seq_t)hc_primes[k];
    }
    seq_t result = 1;
    seq_t rem = N;
    for (int k = 0; k < HC_NP && rem > 0; ++k) {
        seq_t p = (seq_t)hc_primes[k];
        seq_t dgt = rem % p;
        rem = rem / p;
        for (seq_t t = 0; t < dgt; ++t) result *= p;
    }
    return result;
}
