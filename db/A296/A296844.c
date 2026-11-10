/*
 * A296844  Decimal expansion of ratio-sum for A296843; see Comments.
 * https://oeis.org/A296844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = n-th decimal digit of Sum_{k>=1} |A296843(k)/A296843(k-1)
 *            - phi|, phi=(1+sqrt(5))/2; A296843: a(k)=a(k-1)+a(k-2)+b(k+1),
 *            a(0)=1, a(1)=2, b = complement of a in integers >= 3 (computed
 *            with exact bignum arithmetic using d(k)=a(k)-phi*a(k-1))
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

#define A296844_B 1000000000u

static int a296844_cmp(const uint32_t *a, const uint32_t *b, size_t L) {
    for (size_t i = L; i > 0; i--) {
        if (a[i - 1] < b[i - 1]) return -1;
        if (a[i - 1] > b[i - 1]) return 1;
    }
    return 0;
}

static void a296844_mul_small(uint32_t *x, size_t L, uint32_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t v = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)(v % A296844_B);
        carry = v / A296844_B;
    }
}

static void a296844_add_small(uint32_t *x, size_t L, uint32_t v) {
    uint64_t carry = v;
    for (size_t i = 0; i < L && carry; i++) {
        uint64_t s = (uint64_t)x[i] + carry;
        x[i] = (uint32_t)(s % A296844_B);
        carry = s / A296844_B;
    }
}

static void a296844_sub(uint32_t *a, const uint32_t *b, size_t L) {
    int64_t borrow = 0;
    for (size_t i = 0; i < L; i++) {
        int64_t v = (int64_t)a[i] - (int64_t)b[i] - borrow;
        if (v < 0) { v += A296844_B; borrow = 1; } else borrow = 0;
        a[i] = (uint32_t)v;
    }
}

static void a296844_add(uint32_t *a, const uint32_t *b, size_t L) {
    uint64_t carry = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t s = (uint64_t)a[i] + b[i] + carry;
        a[i] = (uint32_t)(s % A296844_B);
        carry = s / A296844_B;
    }
}

/* o = a*m, a has la limbs, o has lo limbs */
static void a296844_mul1(const uint32_t *a, size_t la, uint32_t m, uint32_t *o, size_t lo) {
    uint64_t c = 0;
    for (size_t i = 0; i < lo; i++) {
        uint64_t v = (i < la ? (uint64_t)a[i] * m : 0) + c;
        o[i] = (uint32_t)(v % A296844_B);
        c = v / A296844_B;
    }
}

/* digits of phi: out[0]=1, M digits total */
static uint8_t *a296844_phi(size_t M) {
    size_t L = M / 9 + 4;
    uint32_t *p = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *r = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *d = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint8_t *s = (uint8_t *)malloc(M);
    uint8_t *out = (uint8_t *)malloc(M);
    for (size_t i = 0; i < M; i++) {
        a296844_mul_small(r, L, 100);
        a296844_add_small(r, L, i == 0 ? 5 : 0);
        memcpy(d, p, L * sizeof(uint32_t));
        a296844_mul_small(d, L, 20);
        uint32_t lo = 0, hi = 9;
        while (lo < hi) {
            uint32_t mid = (lo + hi + 1) / 2;
            memcpy(t, d, L * sizeof(uint32_t));
            a296844_add_small(t, L, mid);
            a296844_mul_small(t, L, mid);
            if (a296844_cmp(t, r, L) <= 0) lo = mid; else hi = mid - 1;
        }
        memcpy(t, d, L * sizeof(uint32_t));
        a296844_add_small(t, L, lo);
        a296844_mul_small(t, L, lo);
        a296844_sub(r, t, L);
        a296844_mul_small(p, L, 10);
        a296844_add_small(p, L, lo);
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
    return out;
}

static uint8_t *a296844_digits = NULL;
static size_t a296844_have = 0;

static void a296844_build(size_t nd) {
    size_t K = (nd + 40) / 9 + 1;
    size_t G = 9 * K;
    size_t LM = K + 5;
    size_t LP = K + 2;
    size_t LD = LM + LP + 2;
    size_t R = LM + 2;
    size_t cap = 6 * G + 200;

    uint8_t *ph = a296844_phi(G + 1);
    uint32_t *P = (uint32_t *)calloc(LP, sizeof(uint32_t));
    for (size_t i = 0; i < G + 1; i++) {
        a296844_mul_small(P, LP, 10);
        a296844_add_small(P, LP, ph[i]);
    }
    free(ph);

    uint32_t *aprev = (uint32_t *)calloc(LM, sizeof(uint32_t));
    uint32_t *acur = (uint32_t *)calloc(LM, sizeof(uint32_t));
    uint32_t *anext = (uint32_t *)calloc(LM, sizeof(uint32_t));
    uint32_t *X = (uint32_t *)calloc(LD, sizeof(uint32_t));
    uint32_t *Y = (uint32_t *)calloc(LD, sizeof(uint32_t));
    uint32_t *Q = (uint32_t *)calloc(LD, sizeof(uint32_t));
    uint32_t *S = (uint32_t *)calloc(LD, sizeof(uint32_t));
    uint32_t *rem = (uint32_t *)calloc(R, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)calloc(R, sizeof(uint32_t));
    uint64_t *a64 = (uint64_t *)calloc(cap, sizeof(uint64_t));
    uint64_t *b64 = (uint64_t *)calloc(cap, sizeof(uint64_t));
    size_t b_size = 0;
    uint64_t next_b = 3;
    size_t a_pos = 0;

    aprev[0] = 1;
    acur[0] = 2;
    a64[0] = 1;
    a64[1] = 2;
    size_t n = 1;

    while (aprev[K + 1] == 0 && n + 3 < cap) {
        /* term n: |a(n)*10^G - P*a(n-1)| / a(n-1) */
        memset(X, 0, LD * sizeof(uint32_t));
        for (size_t i = 0; i < LM && K + i < LD; i++) X[K + i] = acur[i];
        memset(Y, 0, LD * sizeof(uint32_t));
        for (size_t i = 0; i < LP; i++) {
            uint64_t carry = 0;
            for (size_t j = 0; j < LM; j++) {
                uint64_t cur = (uint64_t)Y[i + j] + (uint64_t)P[i] * aprev[j] + carry;
                Y[i + j] = (uint32_t)(cur % A296844_B);
                carry = cur / A296844_B;
            }
            Y[i + LM] = (uint32_t)carry;
        }
        if (a296844_cmp(X, Y, LD) >= 0) a296844_sub(X, Y, LD);
        else { a296844_sub(Y, X, LD); memcpy(X, Y, LD * sizeof(uint32_t)); }

        /* Q = X / aprev */
        memset(rem, 0, R * sizeof(uint32_t));
        for (size_t idx = LD; idx > 0; idx--) {
            for (size_t k = R - 1; k >= 1; k--) rem[k] = rem[k - 1];
            rem[0] = X[idx - 1];
            uint32_t lo = 0, hi = A296844_B - 1;
            while (lo < hi) {
                uint32_t mid = lo + (hi - lo + 1) / 2;
                a296844_mul1(aprev, LM, mid, tmp, R);
                if (a296844_cmp(tmp, rem, R) <= 0) lo = mid; else hi = mid - 1;
            }
            if (lo) {
                a296844_mul1(aprev, LM, lo, tmp, R);
                a296844_sub(rem, tmp, R);
            }
            Q[idx - 1] = lo;
        }
        a296844_add(S, Q, LD);

        /* next a(n+1) = a(n) + a(n-1) + b(n+2) */
        size_t need = n + 2;
        while (b_size <= need) {
            uint64_t cand = next_b;
            while (a_pos < n + 1 && a64[a_pos] < cand) ++a_pos;
            if (a_pos < n + 1 && a64[a_pos] == cand) { ++next_b; continue; }
            b64[b_size++] = cand;
            ++next_b;
        }
        memcpy(anext, acur, LM * sizeof(uint32_t));
        a296844_add(anext, aprev, LM);
        a296844_add_small(anext, LM, (uint32_t)b64[need]);
        n++;
        if (anext[3] != 0 || anext[2] >= 9)
            a64[n] = UINT64_MAX;
        else
            a64[n] = (uint64_t)anext[0] + (uint64_t)anext[1] * A296844_B
                     + (uint64_t)anext[2] * A296844_B * A296844_B;
        uint32_t *t = aprev; aprev = acur; acur = anext; anext = t;
    }

    uint8_t *dg = (uint8_t *)malloc(nd);
    uint32_t pw[9] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};
    for (size_t i = 1; i <= nd; i++) {
        size_t pos = G + 1 - i;
        dg[i - 1] = (uint8_t)((S[pos / 9] / pw[pos % 9]) % 10);
    }

    free(P); free(aprev); free(acur); free(anext);
    free(X); free(Y); free(Q); free(S); free(rem); free(tmp);
    free(a64); free(b64);
    free(a296844_digits);
    a296844_digits = dg;
    a296844_have = nd;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if ((size_t)n > a296844_have) {
        size_t nd = a296844_have * 2;
        if (nd < (size_t)n + 30) nd = (size_t)n + 30;
        if (nd < 100) nd = 100;
        a296844_build(nd);
    }
    return (seq_t)a296844_digits[n - 1];
}
