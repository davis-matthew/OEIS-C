/*
 * A296476  Decimal expansion of limiting power-ratio for A295949; see
 *          Comments.
 * https://oeis.org/A296476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   L = lim a(n)/phi^n for A295949, a(n) = a(n-1)+a(n-2)+b(n),
 *            a(0)=1, a(1)=2, b = complement of a (b(0)=3, b(1)=4, b(2)=5); L
 *            = (1 + phi + sum_{k>=2} b(k)*phi^(1-k))/sqrt(5), evaluated in
 *            big fixed point, a(n) = n-th decimal digit of L = 6.1363
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define LPR_BASE 1000000000u

/* ---- golden ratio digits (from A001622, renamed) ---- */
static int phi_cmp(const uint32_t *a, const uint32_t *b, size_t L) {
    for (size_t i = L; i > 0; i--) {
        if (a[i - 1] < b[i - 1]) return -1;
        if (a[i - 1] > b[i - 1]) return 1;
    }
    return 0;
}
static void phi_mul_small(uint32_t *x, size_t L, uint32_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t v = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)(v % LPR_BASE);
        carry = v / LPR_BASE;
    }
}
static void phi_add_small(uint32_t *x, size_t L, uint32_t v) {
    uint64_t carry = v;
    for (size_t i = 0; i < L && carry; i++) {
        uint64_t s = (uint64_t)x[i] + carry;
        x[i] = (uint32_t)(s % LPR_BASE);
        carry = s / LPR_BASE;
    }
}
static void phi_sub(uint32_t *a, const uint32_t *b, size_t L) {
    int64_t borrow = 0;
    for (size_t i = 0; i < L; i++) {
        int64_t v = (int64_t)a[i] - (int64_t)b[i] - borrow;
        if (v < 0) { v += LPR_BASE; borrow = 1; } else borrow = 0;
        a[i] = (uint32_t)v;
    }
}
static uint8_t *phi_digits = NULL;
static size_t phi_have = 0;
static void phi_compute(size_t M) {
    size_t L = M / 9 + 4;
    uint32_t *p = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *r = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *d = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint8_t *s = (uint8_t *)malloc(M);
    uint8_t *out = (uint8_t *)malloc(M);
    for (size_t i = 0; i < M; i++) {
        phi_mul_small(r, L, 100);
        phi_add_small(r, L, i == 0 ? 5 : 0);
        memcpy(d, p, L * sizeof(uint32_t));
        phi_mul_small(d, L, 20);
        uint32_t lo = 0, hi = 9;
        while (lo < hi) {
            uint32_t mid = (lo + hi + 1) / 2;
            memcpy(t, d, L * sizeof(uint32_t));
            phi_add_small(t, L, mid);
            phi_mul_small(t, L, mid);
            if (phi_cmp(t, r, L) <= 0) lo = mid; else hi = mid - 1;
        }
        memcpy(t, d, L * sizeof(uint32_t));
        phi_add_small(t, L, lo);
        phi_mul_small(t, L, lo);
        phi_sub(r, t, L);
        phi_mul_small(p, L, 10);
        phi_add_small(p, L, lo);
        s[i] = (uint8_t)lo;
    }
    s[0] += 1;
    unsigned rem = 0;
    for (size_t i = 0; i < M; i++) {
        unsigned v = rem * 10 + s[i];
        out[i] = (uint8_t)(v / 2);
        rem = v % 2;
    }
    free(p); free(r); free(d); free(t); free(s);
    free(phi_digits);
    phi_digits = out;
    phi_have = M;
}

/* ---- fixed point multiply: r = x*y, W limbs, F fractional limbs ---- */
static void lpr_fmul(uint32_t *r, const uint32_t *x, const uint32_t *y, size_t W, size_t F) {
    uint32_t *tmp = (uint32_t *)calloc(2 * W + 2, sizeof(uint32_t));
    for (size_t i = 0; i < W; i++) {
        uint64_t carry = 0;
        if (x[i] == 0) continue;
        for (size_t j = 0; j < W; j++) {
            uint64_t v = (uint64_t)tmp[i + j] + (uint64_t)x[i] * y[j] + carry;
            tmp[i + j] = (uint32_t)(v % LPR_BASE);
            carry = v / LPR_BASE;
        }
        tmp[i + W] = (uint32_t)carry;
    }
    for (size_t k = 0; k < W; k++) r[k] = tmp[F + k];
    free(tmp);
}

static uint8_t *lpr_digits = NULL;
static size_t lpr_have = 0;

static void lpr_compute(size_t D) {
    size_t F = (D + 30) / 9 + 2;
    size_t W = F + 2;
    size_t K = (D + 25) * 48 / 10 + 10;

    /* b(k) for k = 2..K */
    uint32_t *bb = (uint32_t *)calloc(K + 2, sizeof(uint32_t));
    uint64_t ar[128];
    size_t na = 2;
    ar[0] = 1; ar[1] = 2;
    uint64_t prev = 4;
    for (size_t k = 2; k <= K; k++) {
        uint64_t cand = prev + 1;
        for (;;) {
            int found = 0;
            for (size_t i = 0; i < na; i++) if (ar[i] == cand) { found = 1; break; }
            if (!found) break;
            cand++;
        }
        prev = cand;
        bb[k] = (uint32_t)cand;
        if (ar[na - 1] < 1000000000000ULL && na < 120) {
            ar[na] = ar[na - 1] + ar[na - 2] + cand;
            na++;
        }
    }

    /* phi and c = phi - 1 in fixed point */
    size_t Md = F * 9 + 9;
    if (phi_have < Md) phi_compute(Md);
    uint32_t *ph = (uint32_t *)calloc(W, sizeof(uint32_t));
    uint32_t *c = (uint32_t *)calloc(W, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(W, sizeof(uint32_t));
    uint32_t *s5 = (uint32_t *)calloc(W, sizeof(uint32_t));
    uint32_t *res = (uint32_t *)calloc(W, sizeof(uint32_t));
    ph[F] = 1;
    for (size_t l = 0; l < F; l++) {
        uint32_t limb = 0;
        for (size_t q = 0; q < 9; q++) {
            size_t j = (F - 1 - l) * 9 + q + 1;
            limb = limb * 10 + phi_digits[j];
        }
        ph[l] = limb;
    }
    memcpy(c, ph, W * sizeof(uint32_t));
    c[F] = 0;

    /* Horner: T = sum b(k) c^(k-1) */
    for (size_t k = K; k >= 2; k--) {
        t[F] += bb[k];
        lpr_fmul(t, t, c, W, F);
    }

    /* E = 1 + phi + T */
    {
        uint64_t carry = 0;
        for (size_t i = 0; i < W; i++) {
            uint64_t v = (uint64_t)t[i] + ph[i] + carry + (i == F ? 1 : 0);
            t[i] = (uint32_t)(v % LPR_BASE);
            carry = v / LPR_BASE;
        }
    }
    /* sqrt5 = 2*phi - 1 */
    {
        uint64_t carry = 0;
        for (size_t i = 0; i < W; i++) {
            uint64_t v = (uint64_t)ph[i] * 2 + carry;
            s5[i] = (uint32_t)(v % LPR_BASE);
            carry = v / LPR_BASE;
        }
        s5[F] -= 1;
    }
    lpr_fmul(res, t, s5, W, F);
    /* divide by 5 */
    {
        uint64_t rem = 0;
        for (size_t i = W; i > 0; i--) {
            uint64_t v = rem * LPR_BASE + res[i - 1];
            res[i - 1] = (uint32_t)(v / 5);
            rem = v % 5;
        }
    }

    size_t nd = 1 + F * 9;
    uint8_t *out = (uint8_t *)malloc(nd);
    out[0] = (uint8_t)res[F];
    for (size_t j = 1; j <= F * 9; j++) {
        size_t limb = F - 1 - (j - 1) / 9;
        size_t pos = (j - 1) % 9;
        uint32_t div = 1;
        for (size_t q = 0; q < 8 - pos; q++) div *= 10;
        out[j] = (uint8_t)((res[limb] / div) % 10);
    }
    free(lpr_digits);
    lpr_digits = out;
    lpr_have = nd - 25;  /* keep guard digits unused */

    free(bb); free(ph); free(c); free(t); free(s5); free(res);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if ((size_t)n > lpr_have) {
        size_t D = (size_t)n < 100 ? 100 : (size_t)n * 2;
        lpr_compute(D);
    }
    return (seq_t)lpr_digits[n - 1];
}
