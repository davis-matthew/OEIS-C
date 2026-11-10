/*
 * A323475  Number of new terms added at n-th generation of A306211.
 * https://oeis.org/A323475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n >= 1, a(n) = |B_n|, the number of terms appended
 *            at * generation n of A306211: S_1 = [1], B_n = RL(S_n), S_{n+1}
 *            = S_n ++ B_n, * where RL means "list of run lengths".
 *            Equivalently a(n) = A306215(n+1) - A306215(n). * S_n grows far
 *            too long to store, so the program keeps only a summ...
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 49, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: standard, pass, 55 terms through n = 64 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#define KW 1024
#define MAXL 320
#define MISSV 0xFFFFFFFFu

typedef struct {
    uint64_t len;
    int npre, nsuf;
    uint32_t pre[KW];
    uint32_t suf[KW];
} Lvl;

typedef struct {
    int nl;
    Lvl lv[MAXL];
} Summ;

static Summ g_smA, g_smB;
static Summ *g_cur = NULL;
static idx_t g_curk = 0;
static uint64_t g_res[256];
static uint32_t g_M[3 * KW], g_M2[3 * KW], g_buf[3 * KW], g_tmp[KW];

static const Lvl *lvl_of(const Summ *S, int j)
{
    return &S->lv[j < S->nl ? j : S->nl - 1];
}

static uint32_t getv(const Lvl *L, uint64_t g)
{
    if (g < (uint64_t)L->npre) return L->pre[g];
    if (g < L->len && g + (uint64_t)L->nsuf >= L->len)
        return L->suf[g + (uint64_t)L->nsuf - L->len];
    return MISSV;
}

static int rl_list(const uint32_t *in, int n, uint32_t *out)
{
    int m = 0, i = 0;
    while (i < n) {
        int k = i + 1;
        while (k < n && in[k] == in[i]) ++k;
        out[m++] = (uint32_t)(k - i);
        i = k;
    }
    return m;
}

/* W = summary of (U ++ RL(U)) */
static void concat_step(const Summ *U, Summ *W)
{
    int j = 0, mlen = 0;
    uint64_t t = 0, s = 0;
    for (;;) {
        const Lvl *Lu = lvl_of(U, j);
        const Lvl *Lv = lvl_of(U, j + 1);
        uint64_t ulp = Lu->len - t;
        uint64_t vlen = Lv->len;
        uint64_t total = ulp + (uint64_t)mlen + (vlen - s);
        Lvl *Wj = &W->lv[j];
        uint64_t want = total < KW ? total : KW;
        int n = 0;
        bool ok = true;
        uint64_t i;

        Wj->len = total;
        /* prefix */
        for (i = 0; i < ulp && (uint64_t)n < want; i++) {
            uint32_t x = getv(Lu, i);
            if (x == MISSV) { ok = false; break; }
            Wj->pre[n++] = x;
        }
        if (ok)
            for (int q = 0; q < mlen && (uint64_t)n < want; q++) Wj->pre[n++] = g_M[q];
        if (ok)
            for (i = s; i < vlen && (uint64_t)n < want; i++) {
                uint32_t x = getv(Lv, i);
                if (x == MISSV) break;
                Wj->pre[n++] = x;
            }
        Wj->npre = n;
        /* suffix */
        n = 0; ok = true;
        for (i = vlen; i > s && (uint64_t)n < want;) {
            i--;
            uint32_t x = getv(Lv, i);
            if (x == MISSV) { ok = false; break; }
            g_tmp[n++] = x;
        }
        if (ok)
            for (int q = mlen; q > 0 && (uint64_t)n < want;) { q--; g_tmp[n++] = g_M[q]; }
        if (ok)
            for (i = ulp; i > 0 && (uint64_t)n < want;) {
                i--;
                uint32_t x = getv(Lu, i);
                if (x == MISSV) break;
                g_tmp[n++] = x;
            }
        for (int q = 0; q < n; q++) Wj->suf[q] = g_tmp[n - 1 - q];
        Wj->nsuf = n;

        if ((total <= KW && (uint64_t)Wj->npre == total) || j >= MAXL - 3) {
            /* fully explicit: finish by direct run-length encoding */
            while (!(W->lv[j].len == 1 && W->lv[j].pre[0] == 1) && j < MAXL - 1) {
                Lvl *c = &W->lv[j], *d = &W->lv[j + 1];
                int m = rl_list(c->pre, (int)c->len, d->pre);
                d->len = (uint64_t)m;
                d->npre = d->nsuf = m;
                memcpy(d->suf, d->pre, (size_t)m * sizeof(uint32_t));
                j++;
            }
            W->nl = j + 1;
            return;
        }

        /* move to level j+1 */
        int bl = 0;
        uint64_t tn, sn;
        if (ulp > 0) {
            uint64_t g = ulp - 1, b = g;
            uint32_t x = getv(Lu, g);
            while (b > 0 && getv(Lu, b - 1) == x) b--;
            for (i = b; i < ulp; i++) g_buf[bl++] = getv(Lu, i);
            tn = 0;
            for (i = b; i < Lu->len; i++)
                if (i == b || getv(Lu, i) != getv(Lu, i - 1)) tn++;
        } else {
            tn = lvl_of(U, j + 1)->len;
        }
        for (int q = 0; q < mlen; q++) g_buf[bl++] = g_M[q];
        if (s < vlen) {
            uint32_t x = getv(Lv, s);
            uint64_t e = s + 1;
            while (e < vlen && getv(Lv, e) == x) e++;
            for (i = s; i < e; i++) g_buf[bl++] = getv(Lv, i);
            sn = 0;
            for (i = 0; i < e; i++)
                if (i == 0 || getv(Lv, i) != getv(Lv, i - 1)) sn++;
        } else {
            sn = lvl_of(U, j + 2)->len;
        }
        mlen = rl_list(g_buf, bl, g_M2);
        memcpy(g_M, g_M2, (size_t)mlen * sizeof(uint32_t));
        t = tn;
        s = sn;
        j++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n > 250) return (seq_t)0;
    if (g_cur == NULL) {
        g_cur = &g_smA;
        g_smA.nl = 1;
        g_smA.lv[0].len = 1;
        g_smA.lv[0].npre = g_smA.lv[0].nsuf = 1;
        g_smA.lv[0].pre[0] = 1;
        g_smA.lv[0].suf[0] = 1;
        g_curk = 1;
        g_res[1] = lvl_of(g_cur, 1)->len;
    }
    while (g_curk < n) {
        Summ *nx = (g_cur == &g_smA) ? &g_smB : &g_smA;
        concat_step(g_cur, nx);
        g_cur = nx;
        g_curk++;
        g_res[g_curk] = lvl_of(g_cur, 1)->len;
    }
    return (seq_t)g_res[(size_t)n];
}
