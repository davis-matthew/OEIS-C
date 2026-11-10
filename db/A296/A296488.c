/*
 * A296488  Decimal expansion of limiting power-ratio for A293076; see
 *          Comments.
 * https://oeis.org/A296488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = n-th decimal digit of L = lim_{m->oo} A293076(m)/phi^m,
 *            where L*sqrt(5) = 3 + 1/phi + Sum_{j>=1} b(j-1)/phi^j,
 *            A293076(m) = A293076(m-1) + A293076(m-2) + b(m-2), A293076(0)=1,
 *            A293076(1)=3, b(0)=2, b(1)=4, and b = complement of A293076 (b
 *            increasing)
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

#define A296488_BASE 1000000000u

/* ---- digits of golden ratio (digit-by-digit square root of 5), M digits ---- */
static int A296488_cmp(const uint32_t *a, const uint32_t *b, size_t L) {
    for (size_t i = L; i > 0; i--) {
        if (a[i - 1] < b[i - 1]) return -1;
        if (a[i - 1] > b[i - 1]) return 1;
    }
    return 0;
}
static void A296488_mul_small(uint32_t *x, size_t L, uint32_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t v = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)(v % A296488_BASE);
        carry = v / A296488_BASE;
    }
}
static void A296488_add_small(uint32_t *x, size_t L, uint32_t v) {
    uint64_t carry = v;
    for (size_t i = 0; i < L && carry; i++) {
        uint64_t s = (uint64_t)x[i] + carry;
        x[i] = (uint32_t)(s % A296488_BASE);
        carry = s / A296488_BASE;
    }
}
static void A296488_sub_inplace(uint32_t *a, const uint32_t *b, size_t L) {
    int64_t borrow = 0;
    for (size_t i = 0; i < L; i++) {
        int64_t v = (int64_t)a[i] - (int64_t)b[i] - borrow;
        if (v < 0) { v += A296488_BASE; borrow = 1; } else borrow = 0;
        a[i] = (uint32_t)v;
    }
}

static uint8_t *A296488_phi_digits(size_t M) {
    size_t L = M / 9 + 4;
    uint32_t *p = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *r = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *d = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint8_t *s = (uint8_t *)malloc(M);
    uint8_t *out = (uint8_t *)malloc(M);
    for (size_t i = 0; i < M; i++) {
        A296488_mul_small(r, L, 100);
        A296488_add_small(r, L, i == 0 ? 5 : 0);
        memcpy(d, p, L * sizeof(uint32_t));
        A296488_mul_small(d, L, 20);
        uint32_t lo = 0, hi = 9;
        while (lo < hi) {
            uint32_t mid = (lo + hi + 1) / 2;
            memcpy(t, d, L * sizeof(uint32_t));
            A296488_add_small(t, L, mid);
            A296488_mul_small(t, L, mid);
            if (A296488_cmp(t, r, L) <= 0) lo = mid; else hi = mid - 1;
        }
        memcpy(t, d, L * sizeof(uint32_t));
        A296488_add_small(t, L, lo);
        A296488_mul_small(t, L, lo);
        A296488_sub_inplace(r, t, L);
        A296488_mul_small(p, L, 10);
        A296488_add_small(p, L, lo);
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

/* fixed point helpers, little-endian base 1e9 limbs */
static void A296488_fadd(uint32_t *a, const uint32_t *b, size_t L) {
    uint32_t carry = 0;
    for (size_t i = 0; i < L; i++) {
        uint32_t v = a[i] + b[i] + carry;
        if (v >= A296488_BASE) { v -= A296488_BASE; carry = 1; } else carry = 0;
        a[i] = v;
    }
}
static void A296488_addmul(uint32_t *S, const uint32_t *t, uint64_t m, size_t L) {
    uint64_t carry = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t v = (uint64_t)S[i] + (uint64_t)t[i] * m + carry;
        S[i] = (uint32_t)(v % A296488_BASE);
        carry = v / A296488_BASE;
    }
}

static unsigned char *A296488_dig = NULL;
static size_t A296488_have = 0;

static void A296488_compute(size_t M) {
    size_t FL = (2 * M + 220) / 9 + 2;
    size_t Lw = FL + 1;
    size_t K = (size_t)(4.8 * (double)(M + 40)) + 30;
    size_t bound = K + 300;

    /* b sequence */
    unsigned char *isA = (unsigned char *)calloc(bound + 2, 1);
    uint32_t *B = (uint32_t *)calloc(K + 2, sizeof(uint32_t));
    unsigned long long a0 = 1, a1 = 3;
    int big = 0;
    isA[1] = 1; isA[3] = 1;
    B[0] = 2; B[1] = 4;
    size_t cand = 4;
    for (size_t i = 2; i < K; i++) {
        if (!big) {
            unsigned long long ai = a1 + a0 + B[i - 2];
            if (ai > bound) big = 1; else isA[ai] = 1;
            a0 = a1; a1 = ai;
        }
        cand++;
        while (cand <= bound && isA[cand]) cand++;
        B[i] = (uint32_t)cand;
    }

    /* phi in fixed point */
    uint8_t *pd = A296488_phi_digits(9 * FL + 1);
    uint32_t *ph = (uint32_t *)calloc(Lw, sizeof(uint32_t));
    ph[FL] = 1;
    for (size_t i = 0; i < FL; i++) {
        size_t idx = 1 + 9 * (FL - 1 - i);
        uint32_t v = 0;
        for (int k = 0; k < 9; k++) v = v * 10 + pd[idx + k];
        ph[i] = v;
    }
    free(pd);

    uint32_t *tp = (uint32_t *)calloc(Lw, sizeof(uint32_t));
    uint32_t *tc = (uint32_t *)calloc(Lw, sizeof(uint32_t));
    uint32_t *tn = (uint32_t *)calloc(Lw, sizeof(uint32_t));
    uint32_t *S = (uint32_t *)calloc(Lw, sizeof(uint32_t));
    tp[FL] = 1;
    memcpy(tc, ph, Lw * sizeof(uint32_t));
    tc[FL] = 0; /* 1/phi = phi - 1 */

    /* S = 3 + 1/phi */
    S[FL] = 3;
    A296488_fadd(S, tc, Lw);
    for (size_t j = 1; j <= K; j++) {
        A296488_addmul(S, tc, B[j - 1], Lw);
        memcpy(tn, tp, Lw * sizeof(uint32_t));
        A296488_sub_inplace(tn, tc, Lw);
        uint32_t *tmp = tp; tp = tc; tc = tn; tn = tmp;
    }

    /* sqrt5 = 2*phi - 1 */
    uint32_t *s5 = (uint32_t *)calloc(Lw, sizeof(uint32_t));
    memcpy(s5, ph, Lw * sizeof(uint32_t));
    A296488_fadd(s5, ph, Lw);
    s5[FL] -= 1;

    /* R = S * s5 */
    size_t RL = 2 * Lw;
    uint32_t *R = (uint32_t *)calloc(RL + 1, sizeof(uint32_t));
    for (size_t i = 0; i < Lw; i++) {
        uint64_t carry = 0;
        uint64_t ai = S[i];
        if (!ai) continue;
        for (size_t j = 0; j < Lw; j++) {
            uint64_t v = (uint64_t)R[i + j] + ai * s5[j] + carry;
            R[i + j] = (uint32_t)(v % A296488_BASE);
            carry = v / A296488_BASE;
        }
        size_t k = i + Lw;
        while (carry) {
            uint64_t v = (uint64_t)R[k] + carry;
            R[k] = (uint32_t)(v % A296488_BASE);
            carry = v / A296488_BASE;
            k++;
        }
    }
    /* divide by 5 */
    {
        uint64_t rem = 0;
        for (size_t i = RL; i > 0; i--) {
            uint64_t v = rem * A296488_BASE + R[i - 1];
            R[i - 1] = (uint32_t)(v / 5);
            rem = v % 5;
        }
    }

    unsigned char *out = (unsigned char *)malloc(M);
    uint32_t p10[9] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};
    out[0] = (unsigned char)(R[2 * FL] % 10);
    for (size_t m = 1; m < M; m++) {
        uint32_t limb = R[2 * FL - 1 - (m - 1) / 9];
        size_t pos = (m - 1) % 9;
        out[m] = (unsigned char)((limb / p10[8 - pos]) % 10);
    }

    free(isA); free(B); free(ph); free(tp); free(tc); free(tn);
    free(S); free(s5); free(R);
    free(A296488_dig);
    A296488_dig = out;
    A296488_have = M;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if ((size_t)n > A296488_have) {
        size_t M = A296488_have * 2;
        if (M < (size_t)n + 20) M = (size_t)n + 20;
        if (M < 120) M = 120;
        A296488_compute(M);
    }
    return (seq_t)A296488_dig[n - 1];
}
