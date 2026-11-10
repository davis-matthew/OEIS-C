/*
 * A296473  Decimal expansion of ratio-sum for A295948; see Comments.
 * https://oeis.org/A296473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   digits of the ratio-sum Sum_{n>=1} (A295948(n)/A295948(n-1) -
 *            phi), phi = (1+sqrt(5))/2, with A295948: a(n) = a(n-1) + a(n-2)
 *            + b(n), a(0)=3, a(1)=4, b(0)=1, b(1)=2, b(2)=5, b = complement
 *            of a. Computed in fixed-point big-integer arithmetic; the OEIS
 *            constant takes each term with its sign: the f...
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

/* ---- copy of A001622 (digits of golden ratio) ---- */
static int A001622_cmp(const uint32_t *a, const uint32_t *b, size_t L) {
    for (size_t i = L; i > 0; i--) {
        if (a[i - 1] < b[i - 1]) return -1;
        if (a[i - 1] > b[i - 1]) return 1;
    }
    return 0;
}

#define A001622_BASE 1000000000u

static void A001622_mul_small(uint32_t *x, size_t L, uint32_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t v = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)(v % A001622_BASE);
        carry = v / A001622_BASE;
    }
}

static void A001622_add_small(uint32_t *x, size_t L, uint32_t v) {
    uint64_t carry = v;
    for (size_t i = 0; i < L && carry; i++) {
        uint64_t s = (uint64_t)x[i] + carry;
        x[i] = (uint32_t)(s % A001622_BASE);
        carry = s / A001622_BASE;
    }
}

static void A001622_sub(uint32_t *a, const uint32_t *b, size_t L) {
    int64_t borrow = 0;
    for (size_t i = 0; i < L; i++) {
        int64_t v = (int64_t)a[i] - (int64_t)b[i] - borrow;
        if (v < 0) { v += A001622_BASE; borrow = 1; } else borrow = 0;
        a[i] = (uint32_t)v;
    }
}

static uint8_t *A001622_digits = NULL;
static size_t A001622_have = 0;

static void A001622_compute(size_t M) {
    size_t L = M / 9 + 4;
    uint32_t *p = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *r = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *d = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint8_t *s = (uint8_t *)malloc(M);
    uint8_t *out = (uint8_t *)malloc(M);

    for (size_t i = 0; i < M; i++) {
        A001622_mul_small(r, L, 100);
        A001622_add_small(r, L, i == 0 ? 5 : 0);
        memcpy(d, p, L * sizeof(uint32_t));
        A001622_mul_small(d, L, 20);
        uint32_t lo = 0, hi = 9;
        while (lo < hi) {
            uint32_t mid = (lo + hi + 1) / 2;
            memcpy(t, d, L * sizeof(uint32_t));
            A001622_add_small(t, L, mid);
            A001622_mul_small(t, L, mid);
            if (A001622_cmp(t, r, L) <= 0) lo = mid; else hi = mid - 1;
        }
        memcpy(t, d, L * sizeof(uint32_t));
        A001622_add_small(t, L, lo);
        A001622_mul_small(t, L, lo);
        A001622_sub(r, t, L);
        A001622_mul_small(p, L, 10);
        A001622_add_small(p, L, lo);
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
    free(A001622_digits);
    A001622_digits = out;
    A001622_have = M;
}

static unsigned A001622_a(uint64_t n) {
    if (n == 0) return 0;
    if ((size_t)n + 5 > A001622_have) {
        size_t M = A001622_have * 2;
        if (M < (size_t)n + 20) M = (size_t)n + 20;
        if (M < 64) M = 64;
        A001622_compute(M);
    }
    return (unsigned)A001622_digits[n - 1];
}

/* ---- ratio-sum computation ---- */
#define RS_B 1000000000u

static uint8_t *rs_digits = NULL;
static size_t rs_have = 0;

static size_t rs_blen(const uint32_t *x, size_t l) {
    while (l > 0 && x[l - 1] == 0) l--;
    return l;
}

static int rs_cmp(const uint32_t *x, const uint32_t *y, size_t l) {
    for (size_t i = l; i > 0; i--) {
        if (x[i - 1] < y[i - 1]) return -1;
        if (x[i - 1] > y[i - 1]) return 1;
    }
    return 0;
}

/* q = floor(c * B^K / d); R is scratch of dl+1 limbs; q has cl+K limbs */
static void rs_divshift(const uint32_t *c, size_t cl, const uint32_t *d, size_t dl,
                        size_t K, uint32_t *q, uint32_t *R) {
    size_t nlen = cl + K;
    for (size_t j = 0; j <= dl; j++) R[j] = 0;
    for (size_t ii = nlen; ii > 0; ii--) {
        size_t i = ii - 1;
        uint32_t limb = (i >= K) ? c[i - K] : 0;
        for (size_t j = dl; j > 0; j--) R[j] = R[j - 1];
        R[0] = limb;
        if (R[dl] == 0 && rs_cmp(R, d, dl) < 0) { q[i] = 0; continue; }
        unsigned __int128 rn = ((unsigned __int128)R[dl] * RS_B + (dl >= 1 ? R[dl - 1] : 0)) * RS_B
                               + (dl >= 2 ? R[dl - 2] : 0);
        unsigned __int128 dn = (unsigned __int128)d[dl - 1] * RS_B + (dl >= 2 ? d[dl - 2] : 0);
        unsigned __int128 qe = rn / dn;
        if (qe > RS_B - 1) qe = RS_B - 1;
        uint64_t qh = (uint64_t)qe;
        uint64_t carry = 0;
        int64_t borrow = 0;
        for (size_t j = 0; j < dl; j++) {
            uint64_t p = qh * d[j] + carry;
            carry = p / RS_B;
            int64_t v = (int64_t)R[j] - (int64_t)(p % RS_B) - borrow;
            if (v < 0) { v += RS_B; borrow = 1; } else borrow = 0;
            R[j] = (uint32_t)v;
        }
        int64_t top = (int64_t)R[dl] - (int64_t)carry - borrow;
        while (top < 0) {
            uint64_t cr = 0;
            for (size_t j = 0; j < dl; j++) {
                uint64_t s = (uint64_t)R[j] + d[j] + cr;
                if (s >= RS_B) { s -= RS_B; cr = 1; } else cr = 0;
                R[j] = (uint32_t)s;
            }
            top += (int64_t)cr;
            qh--;
        }
        R[dl] = (uint32_t)top;
        while (R[dl] > 0 || rs_cmp(R, d, dl) >= 0) {
            int64_t bw = 0;
            for (size_t j = 0; j < dl; j++) {
                int64_t v = (int64_t)R[j] - (int64_t)d[j] - bw;
                if (v < 0) { v += RS_B; bw = 1; } else bw = 0;
                R[j] = (uint32_t)v;
            }
            R[dl] = (uint32_t)((int64_t)R[dl] - bw);
            qh++;
        }
        q[i] = (uint32_t)qh;
    }
}

static int rs_in_small(const uint64_t *na, size_t cnt, uint64_t v) {
    for (size_t i = 0; i < cnt; i++) if (na[i] == v) return 1;
    return 0;
}

static void rs_add_into(uint32_t *S, const uint32_t *T, size_t W) {
    uint64_t cr = 0;
    for (size_t j = 0; j < W + 1; j++) {
        uint64_t s = (uint64_t)S[j] + (j < W ? T[j] : 0) + cr;
        if (s >= RS_B) { s -= RS_B; cr = 1; } else cr = 0;
        S[j] = (uint32_t)s;
    }
}

static void rs_compute(size_t want) {
    size_t K = (want + 25) / 9 + 2;
    size_t W = K + 1;
    size_t M = 9 * K * 5 + 60;
    size_t Lmax = M / 40 + 6;
    uint32_t *A0 = (uint32_t *)calloc(Lmax + 2, sizeof(uint32_t));
    uint32_t *A1 = (uint32_t *)calloc(Lmax + 2, sizeof(uint32_t));
    uint32_t *A2 = (uint32_t *)calloc(Lmax + 2, sizeof(uint32_t));
    uint32_t *Q = (uint32_t *)calloc(Lmax + K + 8, sizeof(uint32_t));
    uint32_t *R = (uint32_t *)calloc(Lmax + 4, sizeof(uint32_t));
    uint32_t *phiF = (uint32_t *)calloc(W + 2, sizeof(uint32_t));
    uint32_t *S = (uint32_t *)calloc(W + 3, sizeof(uint32_t));
    uint32_t *Sn = (uint32_t *)calloc(W + 3, sizeof(uint32_t));
    uint32_t *T = (uint32_t *)calloc(W + 3, sizeof(uint32_t));
    uint64_t na[60];
    size_t nacnt = 2;

    /* phi * B^K */
    (void)A001622_a((uint64_t)(9 * K + 1));
    phiF[K] = 1;
    for (size_t t = K; t > 0; t--) {
        size_t limb = t - 1;
        size_t base = 1 + 9 * (K - 1 - limb);
        uint32_t v = 0;
        for (size_t e = 0; e < 9; e++) v = v * 10 + A001622_a((uint64_t)(base + e + 1));
        phiF[limb] = v;
    }

    uint32_t *p = A0, *c = A1, *nx = A2;
    p[0] = 3; c[0] = 4;
    size_t pl = 1, cl = 1;
    na[0] = 3; na[1] = 4;
    uint64_t lastb = 5;

    for (size_t n = 1; n <= M; n++) {
        /* signed term c/p - phi */
        rs_divshift(c, cl, p, pl, K, Q, R);
        for (size_t j = cl + K; j < W + 1; j++) Q[j] = 0;
        if (rs_cmp(Q, phiF, W) >= 0) {
            int64_t bw = 0;
            for (size_t j = 0; j < W; j++) {
                int64_t v = (int64_t)Q[j] - (int64_t)phiF[j] - bw;
                if (v < 0) { v += RS_B; bw = 1; } else bw = 0;
                T[j] = (uint32_t)v;
            }
            rs_add_into(S, T, W);
        } else {
            int64_t bw = 0;
            for (size_t j = 0; j < W; j++) {
                int64_t v = (int64_t)phiF[j] - (int64_t)Q[j] - bw;
                if (v < 0) { v += RS_B; bw = 1; } else bw = 0;
                T[j] = (uint32_t)v;
            }
            rs_add_into(Sn, T, W);
        }
        if (n == M) break;

        /* next a(k), k = n+1 */
        size_t k = n + 1;
        uint64_t b;
        if (k == 2) {
            b = 5;
        } else {
            uint64_t cand = lastb + 1;
            while (rs_in_small(na, nacnt, cand)) cand++;
            b = cand;
        }
        lastb = b;
        {
            uint64_t cr = b;
            size_t lim = cl + 1;
            for (size_t j = 0; j < lim; j++) {
                uint64_t s = (uint64_t)(j < cl ? c[j] : 0) + (j < pl ? p[j] : 0) + cr;
                nx[j] = (uint32_t)(s % RS_B);
                cr = s / RS_B;
            }
            nx[lim] = (uint32_t)cr;
        }
        if (k < 60) {
            na[k] = na[k - 1] + na[k - 2] + b;
            nacnt = k + 1;
        }
        {
            uint32_t *tmp = p;
            p = c; c = nx; nx = tmp;
            pl = cl;
            cl = rs_blen(c, cl + 2);
        }
    }

    /* S = S - Sn (positive part minus negative part) */
    {
        int64_t bw = 0;
        for (size_t j = 0; j < W + 1; j++) {
            int64_t v = (int64_t)S[j] - (int64_t)Sn[j] - bw;
            if (v < 0) { v += RS_B; bw = 1; } else bw = 0;
            S[j] = (uint32_t)v;
        }
    }

    size_t nd = 9 * K + 1;
    uint8_t *out = (uint8_t *)malloc(nd);
    out[0] = (uint8_t)(S[K] % 10);
    size_t pos = 1;
    for (size_t t = K; t > 0; t--) {
        uint32_t v = S[t - 1];
        for (int e = 8; e >= 0; e--) {
            out[pos + (size_t)e] = (uint8_t)(v % 10);
            v /= 10;
        }
        pos += 9;
    }

    free(A0); free(A1); free(A2); free(Q); free(R); free(phiF); free(S); free(Sn); free(T);
    free(rs_digits);
    rs_digits = out;
    rs_have = want;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if ((size_t)n > rs_have) {
        size_t want = rs_have * 2;
        if (want < (size_t)n) want = (size_t)n;
        if (want < 100) want = 100;
        rs_compute(want);
    }
    return (seq_t)rs_digits[(size_t)n - 1];
}
