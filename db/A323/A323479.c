/*
 * A323479  Irregular triangle read by rows: T(n,k) (n>=1) = number of
 *          occurrences of k in n-th generation of A306211.
 * https://oeis.org/A323479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   T(n,k) = number of k's in generation n of A306211, where
 *            generation 1 = [1] and generation g+1 = generation g followed by
 *            its run lengths. Generations grow exponentially, so they are
 *            stored as a shared grammar (DAG of concatenations). For each
 *            node the code stores its run structure, its run-lengt...
 * Range:     u8 n <= 32, u16 n <= 92, u32 n <= 217, u64 n <= 247,
 *            u128 n <= 247, big any n
 * Validated: standard, pass, 79 terms through n = 240 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

typedef struct {
    int l, r;              /* children; l < 0 means leaf */
    uint64_t v, maxv;      /* leaf value, max value in subtree */
    int hr;                /* run info computed */
    uint64_t fv, fl, lv, ll; /* first run (value,len), last run (value,len) */
    int single;            /* whole string is a single run */
    int inner;             /* node for run lengths strictly between first and last run, -1 = empty */
    int rn;                /* node of R(this), -2 = not computed */
    seq_t *cnt;            /* counts by value, length clen */
    size_t clen;
} A323479_nd;

static A323479_nd *A323479_N = NULL;
static int A323479_nn = 0, A323479_ncap = 0;
static int A323479_lcache[4096];
static int A323479_init = 0;

static seq_t *A323479_flat = NULL;
static size_t A323479_flen = 0, A323479_fcap = 0;
static int A323479_S = -1;

static int A323479_new_node(void)
{
    if (A323479_nn == A323479_ncap) {
        A323479_ncap = A323479_ncap ? A323479_ncap * 2 : 1024;
        A323479_N = (A323479_nd *)realloc(A323479_N, (size_t)A323479_ncap * sizeof(A323479_nd));
    }
    A323479_nd *p = &A323479_N[A323479_nn];
    p->l = -1; p->r = -1; p->v = 0; p->maxv = 0;
    p->hr = 0; p->fv = p->fl = p->lv = p->ll = 0; p->single = 0; p->inner = -1;
    p->rn = -2; p->cnt = NULL; p->clen = 0;
    return A323479_nn++;
}

static int A323479_leaf(uint64_t v)
{
    if (v < 4096 && A323479_lcache[v]) return A323479_lcache[v] - 1;
    int x = A323479_new_node();
    A323479_N[x].v = v;
    A323479_N[x].maxv = v;
    if (v < 4096) A323479_lcache[v] = x + 1;
    return x;
}

static int A323479_cat(int a, int b)
{
    if (a < 0) return b;
    if (b < 0) return a;
    int x = A323479_new_node();
    A323479_N[x].l = a;
    A323479_N[x].r = b;
    A323479_N[x].maxv = A323479_N[a].maxv > A323479_N[b].maxv ? A323479_N[a].maxv : A323479_N[b].maxv;
    return x;
}

static void A323479_rinfo(int x)
{
    if (A323479_N[x].hr) return;
    if (A323479_N[x].l < 0) {
        A323479_nd *p = &A323479_N[x];
        p->fv = p->lv = p->v; p->fl = p->ll = 1; p->single = 1; p->inner = -1; p->hr = 1;
        return;
    }
    int A = A323479_N[x].l, B = A323479_N[x].r;
    A323479_rinfo(A);
    A323479_rinfo(B);
    uint64_t afv = A323479_N[A].fv, afl = A323479_N[A].fl, alv = A323479_N[A].lv, all = A323479_N[A].ll;
    int as = A323479_N[A].single, aI = A323479_N[A].inner;
    uint64_t bfv = A323479_N[B].fv, bfl = A323479_N[B].fl, blv = A323479_N[B].lv, bll = A323479_N[B].ll;
    int bs = A323479_N[B].single, bI = A323479_N[B].inner;

    uint64_t fv, fl, lv, ll;
    int single = 0, inner = -1;
    if (alv != bfv) {
        fv = afv; fl = afl; lv = blv; ll = bll;
        if (as && bs) {
            inner = -1;
        } else if (as) {
            inner = A323479_cat(A323479_leaf(bfl), bI);
        } else if (bs) {
            inner = A323479_cat(aI, A323479_leaf(all));
        } else {
            int t = A323479_cat(aI, A323479_leaf(all));
            t = A323479_cat(t, A323479_leaf(bfl));
            inner = A323479_cat(t, bI);
        }
    } else {
        uint64_t m = all + bfl;
        if (as && bs) {
            single = 1; fv = lv = afv; fl = ll = m; inner = -1;
        } else if (as) {
            fv = afv; fl = m; lv = blv; ll = bll; inner = bI;
        } else if (bs) {
            fv = afv; fl = afl; lv = alv; ll = m; inner = aI;
        } else {
            fv = afv; fl = afl; lv = blv; ll = bll;
            int t = A323479_cat(aI, A323479_leaf(m));
            inner = A323479_cat(t, bI);
        }
    }
    A323479_nd *p = &A323479_N[x];
    p->fv = fv; p->fl = fl; p->lv = lv; p->ll = ll;
    p->single = single; p->inner = inner; p->hr = 1;
}

static int A323479_rnode(int x)
{
    if (A323479_N[x].rn != -2) return A323479_N[x].rn;
    A323479_rinfo(x);
    int r;
    if (A323479_N[x].single) {
        r = A323479_leaf(A323479_N[x].fl);
    } else {
        uint64_t fl = A323479_N[x].fl, ll = A323479_N[x].ll;
        int inner = A323479_N[x].inner;
        int t = A323479_cat(A323479_leaf(fl), inner);
        r = A323479_cat(t, A323479_leaf(ll));
    }
    A323479_N[x].rn = r;
    return r;
}

static void A323479_counts(int x)
{
    if (A323479_N[x].cnt) return;
    if (A323479_N[x].l < 0) {
        size_t len = (size_t)A323479_N[x].v + 1;
        seq_t *c = mem_new(seq_t, len);
        c[len - 1] = 1;
        A323479_N[x].cnt = c;
        A323479_N[x].clen = len;
        return;
    }
    int L = A323479_N[x].l, R = A323479_N[x].r;
    A323479_counts(L);
    A323479_counts(R);
    size_t len = (size_t)A323479_N[x].maxv + 1;
    seq_t *c = mem_new(seq_t, len);
    size_t i;
    for (i = 0; i < A323479_N[L].clen; i++) c[i] += A323479_N[L].cnt[i];
    for (i = 0; i < A323479_N[R].clen; i++) c[i] += A323479_N[R].cnt[i];
    A323479_N[x].cnt = c;
    A323479_N[x].clen = len;
}

static void A323479_push_row(int s)
{
    A323479_counts(s);
    size_t len = A323479_N[s].clen;
    if (A323479_flen + len > A323479_fcap) {
        size_t nc = A323479_fcap ? A323479_fcap : 64;
        while (nc < A323479_flen + len) nc *= 2;
        A323479_flat = mem_resize(seq_t, A323479_flat, nc);
        A323479_fcap = nc;
    }
    size_t k;
    for (k = 1; k < len; k++) A323479_flat[A323479_flen++] = A323479_N[s].cnt[k];
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (!A323479_init) {
        A323479_init = 1;
        A323479_S = A323479_leaf(1);
        A323479_push_row(A323479_S);
    }
    while (A323479_flen < (size_t)n) {
        int r = A323479_rnode(A323479_S);
        A323479_S = A323479_cat(A323479_S, r);
        A323479_push_row(A323479_S);
    }
    return A323479_flat[(size_t)n - 1];
}
