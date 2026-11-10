/*
 * A293556  Records in A243822.
 * https://oeis.org/A293556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th record value of A243822(m) = S(m) - tau(m), where
 *            S(m) = #{k <= m : rad(k) | m}. Search over m in dyadic blocks
 *            [X, 2X): go through the prime sets of m depth-first, skip any
 *            branch whose bound (a count of smooth numbers) cannot beat the
 *            current record, and for the prime sets that rema...
 * Range:     u8 n <= 24, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Validated: standard, pass, 70 terms through n = 110 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- primes on demand ---------- */
static uint64_t *A293556_pr = 0;
static size_t A293556_prn = 0, A293556_prcap = 0;

static uint64_t A293556_prime_at(size_t i)
{
    while (A293556_prn <= i) {
        uint64_t c = A293556_prn ? A293556_pr[A293556_prn - 1] + 1 : 2;
        for (;; c++) {
            bool ok = true;
            size_t j;
            for (j = 0; j < A293556_prn; j++) {
                uint64_t q = A293556_pr[j];
                if (q * q > c) break;
                if (c % q == 0) { ok = false; break; }
            }
            if (ok) break;
        }
        if (A293556_prn == A293556_prcap) {
            A293556_prcap = A293556_prcap ? 2 * A293556_prcap : 256;
            A293556_pr = (uint64_t *)realloc(A293556_pr, A293556_prcap * sizeof(uint64_t));
        }
        A293556_pr[A293556_prn++] = c;
    }
    return A293556_pr[i];
}

/* ---------- capped smooth counting ---------- */
static uint64_t A293556_cval, A293556_ccap;
static int A293556_cabort;

static void A293556_cnt_rec(const uint64_t *ps, int k, int idx, uint64_t y)
{
    if (A293556_cabort) return;
    if (idx == k - 1) {
        uint64_t c = 1, t = y, p = ps[idx];
        while (t >= p) { t /= p; c++; }
        A293556_cval += c;
        if (A293556_cval > A293556_ccap) A293556_cabort = 1;
        return;
    }
    {
        uint64_t t = y;
        for (;;) {
            A293556_cnt_rec(ps, k, idx + 1, t);
            if (A293556_cabort) return;
            if (t < ps[idx]) break;
            t /= ps[idx];
        }
    }
}

/* true iff #{m <= Y : m is ps-smooth} > thr ; ps ascending */
static bool A293556_exceeds(const uint64_t *ps, int k, uint64_t Y, uint64_t thr)
{
    uint64_t d[64];
    int i;
    if (k == 0) return 1 > thr;
    for (i = 0; i < k; i++) d[i] = ps[k - 1 - i]; /* descending */
    A293556_cval = 0;
    A293556_ccap = thr;
    A293556_cabort = 0;
    A293556_cnt_rec(d, k, 0, Y);
    return A293556_cval > thr;
}

/* ---------- exact enumeration ---------- */
typedef struct { uint64_t v; uint64_t tau; int used; } A293556_item;
static A293556_item *A293556_it = 0;
static size_t A293556_itn = 0, A293556_itcap = 0;

static void A293556_gen(const uint64_t *ps, int k, int idx, uint64_t v,
                        uint64_t tau, int used, uint64_t Y)
{
    if (idx == k) {
        if (A293556_itn == A293556_itcap) {
            A293556_itcap = A293556_itcap ? 2 * A293556_itcap : 1024;
            A293556_it = (A293556_item *)realloc(A293556_it, A293556_itcap * sizeof(A293556_item));
        }
        A293556_it[A293556_itn].v = v;
        A293556_it[A293556_itn].tau = tau;
        A293556_it[A293556_itn].used = used;
        A293556_itn++;
        return;
    }
    {
        uint64_t p = ps[idx], w = v, e = 0;
        for (;;) {
            A293556_gen(ps, k, idx + 1, w, tau * (e + 1), used + (e > 0), Y);
            if (w > Y / p) break;
            w *= p;
            e++;
        }
    }
}

static int A293556_cmp_item(const void *a, const void *b)
{
    uint64_t x = ((const A293556_item *)a)->v, y = ((const A293556_item *)b)->v;
    return (x > y) - (x < y);
}

typedef struct { uint64_t n; uint64_t f; } A293556_cand;
static A293556_cand *A293556_cd = 0;
static size_t A293556_cdn = 0, A293556_cdcap = 0;

static int A293556_cmp_cand(const void *a, const void *b)
{
    uint64_t x = ((const A293556_cand *)a)->n, y = ((const A293556_cand *)b)->n;
    return (x > y) - (x < y);
}

static void A293556_enumerate(const uint64_t *ps, int k, uint64_t X, uint64_t Y, uint64_t R)
{
    size_t i;
    A293556_itn = 0;
    A293556_gen(ps, k, 0, 1, 1, 0, Y);
    qsort(A293556_it, A293556_itn, sizeof(A293556_item), A293556_cmp_item);
    for (i = 0; i < A293556_itn; i++) {
        A293556_item *t = &A293556_it[i];
        if (t->used == k && t->v >= X) {
            uint64_t s = (uint64_t)(i + 1);
            if (s > t->tau && s - t->tau > R) {
                if (A293556_cdn == A293556_cdcap) {
                    A293556_cdcap = A293556_cdcap ? 2 * A293556_cdcap : 256;
                    A293556_cd = (A293556_cand *)realloc(A293556_cd, A293556_cdcap * sizeof(A293556_cand));
                }
                A293556_cd[A293556_cdn].n = t->v;
                A293556_cd[A293556_cdn].f = s - t->tau;
                A293556_cdn++;
            }
        }
    }
}

/* ---------- DFS over prime sets ---------- */
static void A293556_dfs(uint64_t *Q, int qk, uint64_t prodQ, size_t next_pi,
                        uint64_t X, uint64_t Y, uint64_t R)
{
    uint64_t tmp[64];
    size_t pi;
    for (pi = next_pi;; pi++) {
        uint64_t p = A293556_prime_at(pi), prod;
        int m, i;
        size_t pj;
        if (prodQ > Y / p) break;
        for (i = 0; i < qk; i++) tmp[i] = Q[i];
        tmp[qk] = p;
        m = qk + 1;
        prod = prodQ * p;
        for (pj = pi + 1; m < 63; pj++) {
            uint64_t r = A293556_prime_at(pj);
            if (prod > Y / r) break;
            prod *= r;
            tmp[m++] = r;
        }
        if (!A293556_exceeds(tmp, m, Y, R + ((uint64_t)1 << (qk + 1))))
            break;
        Q[qk] = p;
        if (A293556_exceeds(Q, qk + 1, Y, R + ((uint64_t)1 << (qk + 1))))
            A293556_enumerate(Q, qk + 1, X, Y, R);
        A293556_dfs(Q, qk + 1, prodQ * p, pi + 1, X, Y, R);
    }
}

/* ---------- records ---------- */
static uint64_t *A293556_rec = 0;
static size_t A293556_recn = 0, A293556_reccap = 0;
static int A293556_block = 0;      /* next block exponent */
static uint64_t A293556_cur = 0;

static void A293556_push_rec(uint64_t v)
{
    if (A293556_recn == A293556_reccap) {
        A293556_reccap = A293556_reccap ? 2 * A293556_reccap : 128;
        A293556_rec = (uint64_t *)realloc(A293556_rec, A293556_reccap * sizeof(uint64_t));
    }
    A293556_rec[A293556_recn++] = v;
}

static void A293556_next_block(void)
{
    int b = A293556_block++;
    uint64_t X = (uint64_t)1 << b, Y = 2 * X - 1, R = A293556_cur;
    uint64_t Q[64];
    size_t i;
    if (b == 0) {               /* n = 1: A243822(1) = 0 */
        A293556_push_rec(0);
        A293556_cur = 0;
        return;
    }
    A293556_cdn = 0;
    A293556_dfs(Q, 0, 1, 0, X, Y, R);
    qsort(A293556_cd, A293556_cdn, sizeof(A293556_cand), A293556_cmp_cand);
    for (i = 0; i < A293556_cdn; i++) {
        if (A293556_cd[i].f > A293556_cur) {
            A293556_cur = A293556_cd[i].f;
            A293556_push_rec(A293556_cur);
        }
    }
}

seq_t a(idx_t n)
{
    while (A293556_recn < (size_t)n && A293556_block < 62)
        A293556_next_block();
    if ((size_t)n > A293556_recn) return (seq_t)0;
    return (seq_t)A293556_rec[n - 1];
}
