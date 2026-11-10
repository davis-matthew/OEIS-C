/*
 * A341337  Distinct values of A343511.
 * https://oeis.org/A341337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   distinct values of A343511, where A343511(n) = 1 + Sum_{d|n,
 *            d<n} A343511(d)^2. A343511(n) depends only on the sorted prime
 *            signature of n. The code runs a best-first search over sorted
 *            signatures, taking the smallest value first, and records each
 *            new distinct value. A value that overflows seq_t...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 15, u64 n <= 23, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

#define A341337_MAXK 24

static seq_t *A341337_gv = NULL;
static unsigned char *A341337_ge = NULL;
static unsigned char *A341337_gk = NULL;
static unsigned char *A341337_gp = NULL;
static unsigned char *A341337_go = NULL;   /* overflow flag */
static size_t A341337_gn = 0, A341337_gcap = 0;
static seq_t *A341337_rs = NULL;
static size_t A341337_rn = 0, A341337_rcap = 0;

static seq_t A341337_value(const unsigned char *e, int k, bool *ovf_out)
{
    size_t D = 1;
    for (int i = 0; i < k; i++) D *= (size_t)e[i] + 1;
    size_t kk = (size_t)(k > 0 ? k : 1);
    unsigned char *dig = (unsigned char *)malloc(D * kk);
    unsigned char *ovf = (unsigned char *)calloc(D, 1);
    seq_t *val = mem_new(seq_t, D);
    for (size_t idx = 0; idx < D; idx++) {
        size_t r = idx;
        for (int i = 0; i < k; i++) {
            dig[idx * kk + i] = (unsigned char)(r % ((size_t)e[i] + 1));
            r /= (size_t)e[i] + 1;
        }
    }
    val[0] = (seq_t)1;
    for (size_t idx = 1; idx < D; idx++) {
        seq_t s = (seq_t)1;
        bool of = false;
        for (size_t j = 0; j < idx; j++) {
            bool ok = true;
            for (int i = 0; i < k; i++)
                if (dig[j * kk + i] > dig[idx * kk + i]) { ok = false; break; }
            if (!ok) continue;
            if (ovf[j]) { of = true; break; }
            seq_t v = val[j];
            seq_t sq = (seq_t)(v * v);
            if (v != 0 && (seq_t)(sq / v) != v) { of = true; break; }
            seq_t t = (seq_t)(s + sq);
            if (t < s) { of = true; break; }
            s = t;
        }
        val[idx] = s;
        ovf[idx] = of ? 1 : 0;
    }
    seq_t res = val[D - 1];
    *ovf_out = ovf[D - 1] != 0;
    mem_free(val);
    free(dig);
    free(ovf);
    return res;
}

static void A341337_push(const unsigned char *e, int k)
{
    for (size_t g = 0; g < A341337_gn; g++) {
        if (A341337_gk[g] != k) continue;
        if (memcmp(A341337_ge + g * A341337_MAXK, e, (size_t)k) == 0) return;
    }
    if (A341337_gn == A341337_gcap) {
        size_t nc = A341337_gcap ? A341337_gcap * 2 : 64;
        seq_t *nv = mem_new(seq_t, nc);
        unsigned char *ne = (unsigned char *)calloc(nc * A341337_MAXK, 1);
        unsigned char *nk = (unsigned char *)calloc(nc, 1);
        unsigned char *np = (unsigned char *)calloc(nc, 1);
        unsigned char *no = (unsigned char *)calloc(nc, 1);
        for (size_t i = 0; i < A341337_gn; i++) nv[i] = A341337_gv[i];
        if (A341337_gn) {
            memcpy(ne, A341337_ge, A341337_gn * A341337_MAXK);
            memcpy(nk, A341337_gk, A341337_gn);
            memcpy(np, A341337_gp, A341337_gn);
            memcpy(no, A341337_go, A341337_gn);
        }
        if (A341337_gv) mem_free(A341337_gv);
        free(A341337_ge); free(A341337_gk); free(A341337_gp); free(A341337_go);
        A341337_gv = nv; A341337_ge = ne; A341337_gk = nk; A341337_gp = np; A341337_go = no;
        A341337_gcap = nc;
    }
    size_t g = A341337_gn++;
    memset(A341337_ge + g * A341337_MAXK, 0, A341337_MAXK);
    memcpy(A341337_ge + g * A341337_MAXK, e, (size_t)k);
    A341337_gk[g] = (unsigned char)k;
    A341337_gp[g] = 0;
    bool of = false;
    A341337_gv[g] = A341337_value(e, k, &of);
    A341337_go[g] = of ? 1 : 0;
}

static void A341337_result_add(seq_t v)
{
    if (A341337_rn == A341337_rcap) {
        size_t nc = A341337_rcap ? A341337_rcap * 2 : 64;
        seq_t *nr = mem_new(seq_t, nc);
        for (size_t i = 0; i < A341337_rn; i++) nr[i] = A341337_rs[i];
        if (A341337_rs) mem_free(A341337_rs);
        A341337_rs = nr;
        A341337_rcap = nc;
    }
    A341337_rs[A341337_rn++] = v;
}

seq_t a(idx_t n)
{
    if (A341337_gn == 0) {
        unsigned char e0[A341337_MAXK];
        memset(e0, 0, sizeof(e0));
        A341337_push(e0, 0);
    }
    while (A341337_rn < (size_t)n) {
        size_t best = (size_t)-1;
        for (size_t g = 0; g < A341337_gn; g++) {
            if (A341337_gp[g]) continue;
            if (best == (size_t)-1) { best = g; continue; }
            bool og = A341337_go[g] != 0, ob = A341337_go[best] != 0;
            if (og != ob) { if (!og) best = g; continue; }
            if (A341337_gv[g] < A341337_gv[best]) best = g;
        }
        A341337_gp[best] = 1;
        if (A341337_rn == 0 || A341337_rs[A341337_rn - 1] != A341337_gv[best])
            A341337_result_add(A341337_gv[best]);
        unsigned char e[A341337_MAXK];
        int k = A341337_gk[best];
        memcpy(e, A341337_ge + best * A341337_MAXK, A341337_MAXK);
        for (int i = 0; i < k; i++) {
            if (i == 0 || e[i - 1] > e[i]) {
                unsigned char f[A341337_MAXK];
                memcpy(f, e, A341337_MAXK);
                f[i]++;
                A341337_push(f, k);
            }
        }
        if (k < A341337_MAXK) {
            unsigned char f[A341337_MAXK];
            memcpy(f, e, A341337_MAXK);
            f[k] = 1;
            A341337_push(f, k + 1);
        }
    }
    return A341337_rs[n - 1];
}
