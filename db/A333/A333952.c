/*
 * A333952  Recursively highly composite numbers: numbers m such that
 *          A067824(m) > A067824(k) for all k < m.
 * https://oeis.org/A333952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. Every m = Prod p_k^e_k has the same
 *            A067824 value as the product of primorials (A025487) with the
 *            same exponent multiset, so only those numbers are checked, in
 *            increasing order, and the positions of new records are kept. For
 *            m > 1, A067824(m) = 2*H(m), where H = A074206...
 * Range:     u8 n <= 15, u16 n <= 44, u32 n <= 137, u64 n <= 291,
 *            u128 n <= 291, big any n
 * Validated: standard, pass, 64 terms through n = 280 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef unsigned __int128 r_u128;
typedef struct { uint64_t w[3]; } r_w192;

#define R_NP 32
#define R_MAXJ 112

static const unsigned R_PR[R_NP] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,
    59,61,67,71,73,79,83,89,97,101,103,107,109,113,127,131};

static r_w192 r_add(r_w192 a, r_w192 b)
{
    r_w192 r;
    uint64_t c;
    r.w[0] = a.w[0] + b.w[0]; c = (r.w[0] < a.w[0]);
    r.w[1] = a.w[1] + b.w[1]; uint64_t c2 = (r.w[1] < a.w[1]);
    r.w[1] += c; c2 += (r.w[1] < c);
    r.w[2] = a.w[2] + b.w[2] + c2;
    return r;
}

static r_w192 r_sub(r_w192 a, r_w192 b)
{
    r_w192 nb;
    nb.w[0] = ~b.w[0]; nb.w[1] = ~b.w[1]; nb.w[2] = ~b.w[2];
    r_w192 one; one.w[0] = 1; one.w[1] = 0; one.w[2] = 0;
    return r_add(a, r_add(nb, one));
}

static r_w192 r_mul(r_w192 a, r_w192 b)
{
    r_w192 r;
    r_u128 p = (r_u128)a.w[0] * b.w[0];
    r.w[0] = (uint64_t)p;
    r_u128 mid = p >> 64;
    p = (r_u128)a.w[0] * b.w[1];
    mid += (uint64_t)p;
    uint64_t hi = (uint64_t)(p >> 64);
    p = (r_u128)a.w[1] * b.w[0];
    mid += (uint64_t)p;
    hi += (uint64_t)(p >> 64);
    r.w[1] = (uint64_t)mid;
    hi += (uint64_t)(mid >> 64);
    hi += a.w[0] * b.w[2] + a.w[1] * b.w[1] + a.w[2] * b.w[0];
    r.w[2] = hi;
    return r;
}

static int r_cmp(const r_w192 *a, const r_w192 *b)
{
    for (int i = 2; i >= 0; i--) {
        if (a->w[i] < b->w[i]) return -1;
        if (a->w[i] > b->w[i]) return 1;
    }
    return 0;
}

typedef struct { r_u128 m; r_w192 h; } r_cand;

static int R_J;
static int R_bw;
static r_w192 *R_bin = NULL;
static r_w192 R_c[R_MAXJ + 1];
static r_w192 R_F[R_NP + 1][R_MAXJ + 1];
static r_cand *R_cs = NULL;
static size_t R_cn = 0, R_ccap = 0;
static r_u128 R_Lim;

static void R_push(r_u128 m, r_w192 h)
{
    if (R_cn == R_ccap) {
        R_ccap = R_ccap ? R_ccap * 2 : 4096;
        R_cs = (r_cand *)realloc(R_cs, R_ccap * sizeof(r_cand));
    }
    R_cs[R_cn].m = m;
    R_cs[R_cn].h = h;
    R_cn++;
}

static void R_gen(int i, int maxe, r_u128 m)
{
    if (i >= R_NP) return;
    r_u128 p = (r_u128)R_PR[i];
    r_w192 *Fp = R_F[i];
    r_w192 *Fc = R_F[i + 1];
    int J = R_J;
    for (int e = 1; e <= maxe; e++) {
        if (m > R_Lim / p) break;
        m *= p;
        r_w192 H; H.w[0] = H.w[1] = H.w[2] = 0;
        for (int j = 1; j <= J; j++) {
            Fc[j] = r_mul(Fp[j], R_bin[(size_t)(e + j - 1) * R_bw + e]);
            H = r_add(H, r_mul(Fc[j], R_c[j]));
        }
        R_push(m, H);
        R_gen(i + 1, e, m);
    }
}

static int R_cmpc(const void *x, const void *y)
{
    const r_cand *a = (const r_cand *)x, *b = (const r_cand *)y;
    if (a->m < b->m) return -1;
    if (a->m > b->m) return 1;
    return 0;
}

static r_u128 *R_rec = NULL;
static size_t R_nrec = 0;

static void R_run(int b)
{
    int J = b;
    R_J = J;
    R_Lim = ((r_u128)1) << b;
    R_bw = 2 * J + 1;
    free(R_bin);
    R_bin = (r_w192 *)calloc((size_t)R_bw * R_bw, sizeof(r_w192));
    for (int n = 0; n < R_bw; n++) {
        R_bin[(size_t)n * R_bw].w[0] = 1;
        for (int k = 1; k <= n; k++)
            R_bin[(size_t)n * R_bw + k] =
                r_add(R_bin[(size_t)(n - 1) * R_bw + k - 1],
                      (k <= n - 1) ? R_bin[(size_t)(n - 1) * R_bw + k] : R_bin[0 * R_bw + 1]);
    }
    for (int m = 1; m <= J; m++) {
        r_w192 s; s.w[0] = s.w[1] = s.w[2] = 0;
        for (int j = m; j <= J; j++) {
            if ((j - m) & 1) s = r_sub(s, R_bin[(size_t)j * R_bw + m]);
            else s = r_add(s, R_bin[(size_t)j * R_bw + m]);
        }
        R_c[m] = s;
    }
    for (int j = 0; j <= J; j++) {
        R_F[0][j].w[0] = 1; R_F[0][j].w[1] = 0; R_F[0][j].w[2] = 0;
    }
    R_cn = 0;
    R_gen(0, J, (r_u128)1);
    qsort(R_cs, R_cn, sizeof(r_cand), R_cmpc);
    free(R_rec);
    R_rec = (r_u128 *)malloc((R_cn + 1) * sizeof(r_u128));
    R_nrec = 0;
    R_rec[R_nrec++] = (r_u128)1;
    r_w192 best; best.w[0] = best.w[1] = best.w[2] = 0;
    for (size_t k = 0; k < R_cn; k++) {
        if (r_cmp(&R_cs[k].h, &best) > 0) {
            best = R_cs[k].h;
            R_rec[R_nrec++] = R_cs[k].m;
        }
    }
}

seq_t a(idx_t n)
{
    static int b = 0;
    if (n == 0) return 0;
    if (b == 0 || (r_u128)n > (r_u128)R_nrec) {
        if (b == 0) b = 16;
        for (;;) {
            R_run(b);
            if ((r_u128)n <= (r_u128)R_nrec) break;
            if (b + 8 > 108) return 0;
            b += 8;
        }
    }
    r_u128 v = R_rec[n - 1];
    uint64_t hi = (uint64_t)(v >> 64), lo = (uint64_t)v;
    seq_t r = 0;
    if (hi) {
        r = (seq_t)hi;
        r <<= 32;
        r <<= 32;
    }
    r += (seq_t)lo;
    return r;
}
