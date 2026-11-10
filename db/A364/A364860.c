/*
 * A364860  Greater of a pair of S-amicable numbers k < m such that s(k) = m
 *          and s(m) = k, where s(k) = A364858(k).
 * https://oeis.org/A364860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = A364858(A364859(n)); search: segmented sieve of s(k) =
 *            Sum_{d|k, d<k, d in S} d (S = {k : s(k) <= k}), pairing each k
 *            with m = s(k) > k when s(m) = k; pairs ordered by lesser k
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct { uint64_t t, src; } A364860_pend;
typedef struct { uint64_t lo, hi; } A364860_pair;

static uint8_t *A364860_bits = NULL;     /* bit k set iff k in S, valid for k < H */
static size_t A364860_bits_bytes = 0;
static uint64_t A364860_H = 0;           /* all k < H processed */
static uint64_t *A364860_acc = NULL;
static uint64_t A364860_dmax = 1;

static A364860_pend **A364860_bk = NULL; /* pending (target, source) by target >> 16 */
static size_t *A364860_bkn = NULL, *A364860_bkc = NULL;
static size_t A364860_bksz = 0;

static A364860_pair *A364860_pairs = NULL;
static size_t A364860_np = 0, A364860_pc = 0;

#define A364860_SH 16
#define A364860_L0 ((uint64_t)1 << A364860_SH)
#define A364860_BMAX ((uint64_t)1 << 20)

static void A364860_ensure_bits(uint64_t R)
{
    size_t need = (size_t)(R / 8 + 2);
    if (need <= A364860_bits_bytes) return;
    size_t nc = A364860_bits_bytes ? A364860_bits_bytes : 4096;
    while (nc < need) nc *= 2;
    uint8_t *nb = (uint8_t *)realloc(A364860_bits, nc);
    if (!nb) abort();
    memset(nb + A364860_bits_bytes, 0, nc - A364860_bits_bytes);
    A364860_bits = nb;
    A364860_bits_bytes = nc;
}

static inline int A364860_bit(uint64_t k) { return (A364860_bits[k >> 3] >> (k & 7)) & 1; }
static inline void A364860_setbit(uint64_t k) { A364860_bits[k >> 3] |= (uint8_t)(1u << (k & 7)); }

static void A364860_push(uint64_t t, uint64_t src)
{
    size_t idx = (size_t)(t >> A364860_SH);
    if (idx >= A364860_bksz) {
        size_t nsz = A364860_bksz ? A364860_bksz : 1024;
        while (nsz <= idx) nsz *= 2;
        A364860_bk = (A364860_pend **)realloc(A364860_bk, nsz * sizeof(A364860_pend *));
        A364860_bkn = (size_t *)realloc(A364860_bkn, nsz * sizeof(size_t));
        A364860_bkc = (size_t *)realloc(A364860_bkc, nsz * sizeof(size_t));
        if (!A364860_bk || !A364860_bkn || !A364860_bkc) abort();
        for (size_t i = A364860_bksz; i < nsz; i++) {
            A364860_bk[i] = NULL; A364860_bkn[i] = 0; A364860_bkc[i] = 0;
        }
        A364860_bksz = nsz;
    }
    if (A364860_bkn[idx] == A364860_bkc[idx]) {
        size_t nc = A364860_bkc[idx] ? A364860_bkc[idx] * 2 : 16;
        A364860_pend *p = (A364860_pend *)realloc(A364860_bk[idx], nc * sizeof(A364860_pend));
        if (!p) abort();
        A364860_bk[idx] = p;
        A364860_bkc[idx] = nc;
    }
    A364860_bk[idx][A364860_bkn[idx]].t = t;
    A364860_bk[idx][A364860_bkn[idx]].src = src;
    A364860_bkn[idx]++;
}

static void A364860_addpair(uint64_t lo, uint64_t hi)
{
    if (A364860_np == A364860_pc) {
        A364860_pc = A364860_pc ? A364860_pc * 2 : 64;
        A364860_pairs = (A364860_pair *)realloc(A364860_pairs, A364860_pc * sizeof(A364860_pair));
        if (!A364860_pairs) abort();
    }
    A364860_pairs[A364860_np].lo = lo;
    A364860_pairs[A364860_np].hi = hi;
    A364860_np++;
}

static void A364860_init(void)
{
    uint64_t L0 = A364860_L0, d, j, k;
    uint64_t *s0 = (uint64_t *)calloc((size_t)L0, sizeof(uint64_t));
    if (!s0) abort();
    A364860_ensure_bits(L0);
    for (d = 1; d < L0; d++) {
        if (s0[d] <= d) {
            A364860_setbit(d);
            for (j = 2 * d; j < L0; j += d) s0[j] += d;
        }
    }
    for (k = 1; k < L0; k++) {
        uint64_t m = s0[k];
        if (m > k) {
            if (m < L0) { if (s0[m] == k) A364860_addpair(k, m); }
            else A364860_push(m, k);
        }
    }
    free(s0);
    A364860_acc = (uint64_t *)malloc((size_t)A364860_BMAX * sizeof(uint64_t));
    if (!A364860_acc) abort();
    A364860_H = L0;
}

static void A364860_segment(void)
{
    uint64_t L = A364860_H;
    uint64_t B = L < A364860_BMAX ? L : A364860_BMAX;
    uint64_t R = L + B, d, n;
    uint64_t *acc = A364860_acc;
    const uint8_t *bits;
    A364860_ensure_bits(R);
    bits = A364860_bits;
    for (n = 0; n < B; n++) acc[n] = 1;           /* divisor 1, in S */
    while ((A364860_dmax + 1) * (A364860_dmax + 1) <= R - 1) A364860_dmax++;
    for (d = 2; d <= A364860_dmax; d++) {
        uint64_t sd = A364860_bit(d) ? d : 0;
        uint64_t j = ((L + d - 1) / d) * d, q;
        if (j < d * d) j = d * d;
        if (j >= R) continue;
        q = j / d;
        if (j == d * d) { acc[j - L] += sd; j += d; q++; }
        for (; j < R; j += d, q++)
            acc[j - L] += sd + (((bits[q >> 3] >> (q & 7)) & 1) ? q : 0);
    }
    for (n = L; n < R; n++) {
        uint64_t v = acc[n - L];
        if (v <= n) A364860_setbit(n);
        else A364860_push(v, n);
    }
    for (size_t b = (size_t)(L >> A364860_SH); b < (size_t)(R >> A364860_SH) && b < A364860_bksz; b++) {
        for (size_t i = 0; i < A364860_bkn[b]; i++) {
            A364860_pend e = A364860_bk[b][i];
            if (e.src && acc[e.t - L] == e.src) A364860_addpair(e.src, e.t);
        }
        free(A364860_bk[b]);
        A364860_bk[b] = NULL; A364860_bkn[b] = 0; A364860_bkc[b] = 0;
    }
    A364860_H = R;
}

static uint64_t A364860_sdir(uint64_t m);

static int A364860_inS(uint64_t d)
{
    if (d < A364860_H) return A364860_bit(d);
    return A364860_sdir(d) <= d;
}

/* direct s(m) from the divisors of m */
static uint64_t A364860_sdir(uint64_t m)
{
    uint64_t pr[64], x = m, p, sum = 0;
    int ex[64], np = 0, i, e;
    size_t cnt = 1, c, t;
    uint64_t *dv;
    if (m <= 1) return 0;
    for (p = 2; p * p <= x; p += (p == 2 ? 1 : 2)) {
        if (x % p == 0) {
            e = 0;
            while (x % p == 0) { x /= p; e++; }
            pr[np] = p; ex[np] = e; np++;
        }
    }
    if (x > 1) { pr[np] = x; ex[np] = 1; np++; }
    for (i = 0; i < np; i++) cnt *= (size_t)(ex[i] + 1);
    dv = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!dv) abort();
    dv[0] = 1; c = 1;
    for (i = 0; i < np; i++) {
        size_t base = c;
        uint64_t pw = 1;
        for (e = 1; e <= ex[i]; e++) {
            pw *= pr[i];
            for (t = 0; t < base; t++) dv[c++] = dv[t] * pw;
        }
    }
    for (t = 0; t < c; t++)
        if (dv[t] < m && A364860_inS(dv[t])) sum += dv[t];
    free(dv);
    return sum;
}

static int A364860_cmp(const void *x, const void *y)
{
    const A364860_pair *a1 = (const A364860_pair *)x, *b1 = (const A364860_pair *)y;
    return a1->lo < b1->lo ? -1 : (a1->lo > b1->lo ? 1 : 0);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (A364860_H == 0) A364860_init();
    for (;;) {
        if ((uint64_t)A364860_np >= (uint64_t)n) {
            uint64_t kn;
            size_t count = 0, b, i;
            qsort(A364860_pairs, A364860_np, sizeof(A364860_pair), A364860_cmp);
            kn = A364860_pairs[n - 1].lo;
            for (b = (size_t)(A364860_H >> A364860_SH); b < A364860_bksz; b++)
                for (i = 0; i < A364860_bkn[b]; i++)
                    if (A364860_bk[b][i].src && A364860_bk[b][i].src < kn) count++;
            if (count == 0) return (seq_t)A364860_pairs[n - 1].hi;
            if (count <= 20000) {
                for (b = (size_t)(A364860_H >> A364860_SH); b < A364860_bksz; b++)
                    for (i = 0; i < A364860_bkn[b]; i++) {
                        A364860_pend *e = &A364860_bk[b][i];
                        if (e->src && e->src < kn) {
                            if (A364860_sdir(e->t) == e->src) A364860_addpair(e->src, e->t);
                            e->src = 0;
                        }
                    }
                continue;
            }
        }
        if (A364860_H >= ((uint64_t)1 << 32)) return 0;
        {
            uint64_t step = A364860_H / 4;
            uint64_t target;
            if (step < A364860_BMAX) step = A364860_BMAX;
            target = A364860_H + step;
            while (A364860_H < target) A364860_segment();
        }
    }
}
