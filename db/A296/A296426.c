/*
 * A296426  Decimal expansion of ratio-sum for A296251; see Comments.
 * https://oeis.org/A296426
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = digits of Sum_{k>=1} |A296251(k)/A296251(k-1) - phi|,
 *            phi=(1+sqrt(5))/2; A296251: a(k)=a(k-1)+a(k-2)+b(k-1)^2,
 *            a(0)=1,a(1)=2,b(0)=3,b(1)=4, a,b complementary increasing;
 *            computed with exact fixed-point bignum arithmetic (base 1e9)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 87 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define RS_B 1000000000u

static int rs_cmp(const uint32_t *x, const uint32_t *y, int L) {
    for (int i = L - 1; i >= 0; i--)
        if (x[i] != y[i]) return x[i] < y[i] ? -1 : 1;
    return 0;
}

static int rs_len(const uint32_t *x, int L) {
    while (L > 0 && x[L - 1] == 0) L--;
    return L;
}

/* T[0..dl+1] = den * m */
static void rs_mulsmall(const uint32_t *den, int dl, uint32_t m, uint32_t *T) {
    uint64_t c = 0;
    for (int i = 0; i < dl; i++) {
        uint64_t v = (uint64_t)den[i] * m + c;
        T[i] = (uint32_t)(v % RS_B);
        c = v / RS_B;
    }
    T[dl] = (uint32_t)c;
    T[dl + 1] = 0;
}

/* out[0..K] = floor(num * B^K / den), den nonzero, quotient < B^(K+1) */
static void rs_divfix(const uint32_t *num, int nl, const uint32_t *den, int dl, int K,
                      uint32_t *out, uint32_t *R, uint32_t *T) {
    int Lc = dl + 2;
    memset(out, 0, (size_t)(K + 1) * sizeof(uint32_t));
    memset(R, 0, (size_t)Lc * sizeof(uint32_t));
    for (int i = nl + K - 1; i >= 0; i--) {
        for (int j = Lc - 1; j > 0; j--) R[j] = R[j - 1];
        R[0] = (i >= K) ? num[i - K] : 0;
        rs_mulsmall(den, dl, 1, T);
        if (rs_cmp(R, T, Lc) < 0) continue;
        uint32_t lo = 1, hi = RS_B - 1;
        while (lo < hi) {
            uint32_t mid = lo + (hi - lo + 1) / 2;
            rs_mulsmall(den, dl, mid, T);
            if (rs_cmp(T, R, Lc) <= 0) lo = mid; else hi = mid - 1;
        }
        rs_mulsmall(den, dl, lo, T);
        int64_t borrow = 0;
        for (int j = 0; j < Lc; j++) {
            int64_t v = (int64_t)R[j] - (int64_t)T[j] - borrow;
            if (v < 0) { v += RS_B; borrow = 1; } else borrow = 0;
            R[j] = (uint32_t)v;
        }
        if (i <= K) out[i] = lo;
    }
}

static uint8_t *rs_digits = NULL;
static size_t rs_usable = 0;

static void rs_compute(size_t D) {
    int K = (int)((D + 40) / 9 + 1);
    int N = (int)((long)9 * K * 55 / 10) + 200;
    int LA = 2 * K + 20;
    int USED = N + 2000;
    size_t sz = (size_t)LA + 8;
    uint32_t *A0 = (uint32_t *)calloc(sz, sizeof(uint32_t));
    uint32_t *A1 = (uint32_t *)calloc(sz, sizeof(uint32_t));
    uint32_t *An = (uint32_t *)calloc(sz, sizeof(uint32_t));
    uint32_t *R = (uint32_t *)calloc(sz, sizeof(uint32_t));
    uint32_t *T = (uint32_t *)calloc(sz, sizeof(uint32_t));
    uint32_t *Q = (uint32_t *)calloc((size_t)K + 4, sizeof(uint32_t));
    uint32_t *PHI = (uint32_t *)calloc((size_t)K + 4, sizeof(uint32_t));
    uint32_t *S = (uint32_t *)calloc((size_t)K + 4, sizeof(uint32_t));
    uint32_t *F0 = (uint32_t *)calloc((size_t)K + 8, sizeof(uint32_t));
    uint32_t *F1 = (uint32_t *)calloc((size_t)K + 8, sizeof(uint32_t));
    uint32_t *Ft = (uint32_t *)calloc((size_t)K + 8, sizeof(uint32_t));
    unsigned char *used = (unsigned char *)calloc((size_t)USED + 2, 1);
    int FL = K + 8;

    /* phi = F(m+1)/F(m) */
    F0[0] = 1; F1[0] = 1;
    while (rs_len(F1, FL) <= K / 2 + 2) {
        uint64_t c = 0;
        for (int i = 0; i < FL; i++) {
            uint64_t v = (uint64_t)F0[i] + F1[i] + c;
            Ft[i] = (uint32_t)(v % RS_B);
            c = v / RS_B;
        }
        memcpy(F0, F1, (size_t)FL * sizeof(uint32_t));
        memcpy(F1, Ft, (size_t)FL * sizeof(uint32_t));
    }
    rs_divfix(F1, rs_len(F1, FL), F0, rs_len(F0, FL), K, PHI, R, T);

    memset(R, 0, sz * sizeof(uint32_t));
    memset(T, 0, sz * sizeof(uint32_t));
    A0[0] = 1; A1[0] = 2;
    for (int i = 1; i <= 4; i++) used[i] = 1;
    uint64_t bprev = 4, cand = 5;

    for (int idx = 1; idx <= N; idx++) {
        const uint32_t *num, *den;
        if (idx == 1) {
            num = A1; den = A0;
        } else {
            /* An = A1 + A0 + bprev^2 */
            uint64_t c = 0;
            for (int i = 0; i < LA; i++) {
                uint64_t v = (uint64_t)A1[i] + A0[i] + c;
                An[i] = (uint32_t)(v % RS_B);
                c = v / RS_B;
            }
            c = bprev * bprev;
            for (int i = 0; i < LA && c; i++) {
                uint64_t v = (uint64_t)An[i] + c;
                An[i] = (uint32_t)(v % RS_B);
                c = v / RS_B;
            }
            if (rs_len(An, LA) <= 1 && An[0] < (uint32_t)USED) used[An[0]] = 1;
            while (used[cand]) cand++;
            used[cand] = 1;
            bprev = cand;
            cand++;
            memcpy(A0, A1, (size_t)LA * sizeof(uint32_t));
            memcpy(A1, An, (size_t)LA * sizeof(uint32_t));
            num = A1; den = A0;
        }
        rs_divfix(num, rs_len(num, LA), den, rs_len(den, LA), K, Q, R, T);
        /* diff = |Q - PHI|, added to S */
        const uint32_t *hiV = Q, *loV = PHI;
        if (rs_cmp(Q, PHI, K + 1) < 0) { hiV = PHI; loV = Q; }
        int64_t borrow = 0;
        uint64_t c = 0;
        for (int i = 0; i <= K; i++) {
            int64_t v = (int64_t)hiV[i] - (int64_t)loV[i] - borrow;
            if (v < 0) { v += RS_B; borrow = 1; } else borrow = 0;
            uint64_t s = (uint64_t)S[i] + (uint64_t)v + c;
            S[i] = (uint32_t)(s % RS_B);
            c = s / RS_B;
        }
    }

    size_t total = 2 + (size_t)9 * K;
    uint8_t *out = (uint8_t *)malloc(total);
    out[0] = (uint8_t)((S[K] / 10) % 10);
    out[1] = (uint8_t)(S[K] % 10);
    for (int j = 0; j < 9 * K; j++) {
        uint32_t v = S[K - 1 - j / 9];
        int p = 8 - (j % 9);
        while (p-- > 0) v /= 10;
        out[2 + j] = (uint8_t)(v % 10);
    }
    free(A0); free(A1); free(An); free(R); free(T); free(Q);
    free(PHI); free(S); free(F0); free(F1); free(Ft); free(used);
    free(rs_digits);
    rs_digits = out;
    rs_usable = total - 40;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;
    size_t m = (size_t)(n - 2);
    if (m >= rs_usable) {
        size_t D = rs_usable * 2;
        if (D < m + 20) D = m + 20;
        if (D < 64) D = 64;
        rs_compute(D);
    }
    return (seq_t)rs_digits[m];
}
