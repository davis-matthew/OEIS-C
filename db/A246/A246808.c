/*
 * A246808  Palindromic terms of the sequence A245317.
 * https://oeis.org/A246808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   search. a(n) = c(k).k is a palindrome, where c(k) is the k-th
 *            composite (A245317). Primes are sieved in segments. In each
 *            segment, the candidates for k come from the digit structure:
 *            c(k) = reversal(k), or reversal(k)*10 + m. A candidate counts
 *            only if k = c - 1 - pi(c)
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define A246808_SEG 32768            /* odd numbers per segment */
#define A246808_PAT 15015            /* 3*5*7*11*13 */
#define A246808_BPLIM 131072u
#define A246808_MAXF 64

static uint64_t A246808_P10[20];
static unsigned char A246808_seg[A246808_SEG];
static uint32_t A246808_chunk[A246808_SEG / 64 + 1];
static unsigned char *A246808_pat = NULL;
static uint32_t *A246808_bp = NULL;
static uint64_t *A246808_nxt = NULL;
static size_t A246808_nbp = 0;
static uint64_t A246808_G0 = 0;
static uint64_t A246808_pi_before = 0;
static int A246808_inited = 0;
static int A246808_exhausted = 0;

static uint64_t A246808_fC[A246808_MAXF], A246808_fN[A246808_MAXF];
static int A246808_fD[A246808_MAXF];
static int A246808_nf = 0;

static int A246808_ndig(uint64_t x)
{
    int d = 1;
    while (x >= 10) { x /= 10; d++; }
    return d;
}

static uint64_t A246808_rev(uint64_t x)
{
    uint64_t r = 0;
    while (x) { r = r * 10 + x % 10; x /= 10; }
    return r;
}

static void A246808_init(void)
{
    uint32_t i, j;
    unsigned char *comp;
    size_t cnt = 0;
    A246808_P10[0] = 1;
    for (i = 1; i < 20; i++) A246808_P10[i] = A246808_P10[i - 1] * 10;

    A246808_pat = (unsigned char *)malloc(A246808_PAT + A246808_SEG);
    for (i = 0; i < A246808_PAT + A246808_SEG; i++) {
        uint64_t x = 2 * (uint64_t)i + 1;
        A246808_pat[i] = (x % 3 && x % 5 && x % 7 && x % 11 && x % 13) ? 1 : 0;
    }

    comp = (unsigned char *)calloc(A246808_BPLIM + 1, 1);
    for (i = 2; (uint64_t)i * i <= A246808_BPLIM; i++)
        if (!comp[i])
            for (j = i * i; j <= A246808_BPLIM; j += i) comp[j] = 1;
    for (i = 17; i <= A246808_BPLIM; i++) if (!comp[i]) cnt++;
    A246808_bp = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    A246808_nxt = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    A246808_nbp = 0;
    for (i = 17; i <= A246808_BPLIM; i++)
        if (!comp[i]) {
            A246808_bp[A246808_nbp] = i;
            A246808_nxt[A246808_nbp] = ((uint64_t)i * i - 1) / 2;
            A246808_nbp++;
        }
    free(comp);
    A246808_inited = 1;
}

/* number of primes <= C, C inside current segment starting at s */
static uint64_t A246808_pi(uint64_t pi_base, uint64_t s, uint64_t C)
{
    uint64_t idx = (C - s + 1) / 2;
    uint64_t c = idx / 64, k;
    uint64_t r = pi_base + A246808_chunk[c];
    for (k = c * 64; k < idx; k++) r += A246808_seg[k];
    return r;
}

static void A246808_segment(void)
{
    uint64_t G0 = A246808_G0;
    uint64_t s = 2 * G0, t = s + 2 * (uint64_t)A246808_SEG - 1;
    uint64_t end = G0 + A246808_SEG;
    uint64_t total = 0, pi_base, n_lo, n_hi, Cs;
    size_t k;
    int c, d, d1, d2;
    uint64_t lc[A246808_MAXF], ln[A246808_MAXF];
    int ld[A246808_MAXF];
    int nl_ = 0, i2, j2;

    if ((uint64_t)A246808_bp[A246808_nbp - 1] * A246808_bp[A246808_nbp - 1] < t) {
        A246808_exhausted = 1;
        return;
    }

    memcpy(A246808_seg, A246808_pat + (size_t)(G0 % A246808_PAT), A246808_SEG);
    if (G0 == 0) {
        A246808_seg[0] = 0;
        A246808_seg[1] = A246808_seg[2] = A246808_seg[3] = 1;
        A246808_seg[5] = A246808_seg[6] = 1;
    }
    for (k = 0; k < A246808_nbp; k++) {
        uint64_t p = A246808_bp[k], g;
        if (p * p > t) break;
        g = A246808_nxt[k];
        while (g < end) { A246808_seg[g - G0] = 0; g += p; }
        A246808_nxt[k] = g;
    }
    for (c = 0; c < A246808_SEG / 64; c++) {
        uint64_t acc = 0, w;
        int q;
        A246808_chunk[c] = (uint32_t)total;
        for (q = 0; q < 8; q++) {
            memcpy(&w, A246808_seg + c * 64 + q * 8, 8);
            acc += w;
        }
        total += (acc * 0x0101010101010101ULL) >> 56;
    }
    A246808_chunk[A246808_SEG / 64] = (uint32_t)total;

    pi_base = A246808_pi_before + (G0 == 0 ? 1 : 0);
    n_hi = t - 1 - (pi_base + total);
    n_lo = (G0 == 0) ? 1 : s - 1 - A246808_pi_before;
    Cs = s < 4 ? 4 : s;

    d1 = A246808_ndig(n_lo);
    d2 = A246808_ndig(n_hi);
    for (d = d1; d <= d2; d++) {
        int j = (d + 1) / 2, h = d - j, sh;
        uint64_t nl = n_lo > A246808_P10[d - 1] ? n_lo : A246808_P10[d - 1];
        uint64_t nh = n_hi < A246808_P10[d] - 1 ? n_hi : A246808_P10[d] - 1;
        if (nl > nh) continue;
        for (sh = 0; sh <= 1; sh++) {
            uint64_t f = A246808_P10[sh], mmax = sh ? 9 : 0;
            uint64_t Rlo, Rhi, Qlo, Qhi, Hlo, Hhi, H, Q;
            Rlo = (Cs > mmax) ? (Cs - mmax + f - 1) / f : 0;
            Rhi = t / f;
            if (Rlo < A246808_P10[d - 1]) Rlo = A246808_P10[d - 1];
            if (Rhi > A246808_P10[d] - 1) Rhi = A246808_P10[d] - 1;
            if (Rlo > Rhi) continue;
            Qlo = Rlo / A246808_P10[h];
            Qhi = Rhi / A246808_P10[h];
            if (Qlo < A246808_P10[j - 1]) Qlo = A246808_P10[j - 1];
            if (Qhi > A246808_P10[j] - 1) Qhi = A246808_P10[j] - 1;
            if (Qlo > Qhi) continue;
            if (h == 0) { Hlo = 0; Hhi = 0; }
            else {
                Hlo = nl / A246808_P10[j];
                if (Hlo < A246808_P10[h - 1]) Hlo = A246808_P10[h - 1];
                Hhi = nh / A246808_P10[j];
                if (Hhi > A246808_P10[h] - 1) Hhi = A246808_P10[h] - 1;
            }
            for (H = Hlo; H <= Hhi; H++) {
                uint64_t rh = (h == 0) ? 0 : A246808_rev(H);
                for (Q = Qlo; Q <= Qhi; Q++) {
                    uint64_t n = H * A246808_P10[j] + A246808_rev(Q);
                    uint64_t R, m;
                    if (n < nl || n > nh) continue;
                    R = Q * A246808_P10[h] + rh;
                    for (m = 0; m <= mmax; m++) {
                        uint64_t C = R * f + m;
                        bool comp;
                        if (C < Cs || C > t) continue;
                        if (C % 2 == 0) comp = (C >= 4);
                        else comp = (C != 1) && !A246808_seg[(C - s - 1) / 2];
                        if (!comp) continue;
                        if (C - 1 - A246808_pi(pi_base, s, C) != n) continue;
                        if (nl_ < A246808_MAXF) {
                            lc[nl_] = C; ln[nl_] = n; ld[nl_] = d; nl_++;
                        }
                    }
                }
            }
        }
    }
    /* sort by C and append */
    for (i2 = 1; i2 < nl_; i2++) {
        uint64_t tc = lc[i2], tn = ln[i2];
        int td = ld[i2];
        j2 = i2 - 1;
        while (j2 >= 0 && lc[j2] > tc) {
            lc[j2 + 1] = lc[j2]; ln[j2 + 1] = ln[j2]; ld[j2 + 1] = ld[j2]; j2--;
        }
        lc[j2 + 1] = tc; ln[j2 + 1] = tn; ld[j2 + 1] = td;
    }
    for (i2 = 0; i2 < nl_ && A246808_nf < A246808_MAXF; i2++) {
        A246808_fC[A246808_nf] = lc[i2];
        A246808_fN[A246808_nf] = ln[i2];
        A246808_fD[A246808_nf] = ld[i2];
        A246808_nf++;
    }

    A246808_pi_before = pi_base + total;
    A246808_G0 = end;
}

seq_t a(idx_t n)
{
    seq_t v;
    int i;
    if (n == 0) return 0;
    if (!A246808_inited) A246808_init();
    while ((idx_t)A246808_nf < n && !A246808_exhausted && A246808_nf < A246808_MAXF)
        A246808_segment();
    if ((idx_t)A246808_nf < n) return 0;
    v = (seq_t)A246808_fC[n - 1];
    for (i = 0; i < A246808_fD[n - 1]; i++) v *= 10;
    v += (seq_t)A246808_fN[n - 1];
    return v;
}
