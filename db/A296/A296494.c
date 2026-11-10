/*
 * A296494  Decimal expansion of limiting power-ratio for A296555; see
 *          Comments.
 * https://oeis.org/A296494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = n-th decimal digit of L = lim_{m->oo} A296555(m)/phi^m,
 *            phi=(1+sqrt(5))/2, where A296555(m) =
 *            A296555(m-1)+A296555(m-2)+b(m)+m, A296555(0)=1, A296555(1)=2,
 *            and b is the increasing complement of A296555
 *            (b(0)=3,b(1)=4,b(2)=5). Computed by exact bignum evaluation of
 *            A296555(N)/(F(N)*phi+F(N-...
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

#define L5_B 1000000000u

static void l5_add(uint32_t *r, const uint32_t *x, const uint32_t *y, size_t L) {
    uint32_t c = 0;
    for (size_t i = 0; i < L; i++) {
        uint32_t v = x[i] + y[i] + c;
        if (v >= L5_B) { v -= L5_B; c = 1; } else c = 0;
        r[i] = v;
    }
}

static void l5_add_small(uint32_t *x, size_t L, uint64_t s) {
    uint64_t c = s;
    for (size_t i = 0; i < L && c; i++) {
        uint64_t v = (uint64_t)x[i] + c;
        x[i] = (uint32_t)(v % L5_B);
        c = v / L5_B;
    }
}

static void l5_mul(uint32_t *r, const uint32_t *x, const uint32_t *y, size_t L) {
    memset(r, 0, L * sizeof(uint32_t));
    for (size_t i = 0; i < L; i++) {
        if (!x[i]) continue;
        uint64_t c = 0;
        for (size_t j = 0; j + i < L; j++) {
            uint64_t v = (uint64_t)r[i + j] + (uint64_t)x[i] * y[j] + c;
            r[i + j] = (uint32_t)(v % L5_B);
            c = v / L5_B;
        }
    }
}

static int l5_cmp(const uint32_t *a, const uint32_t *b, size_t L) {
    for (size_t i = L; i > 0; i--) {
        if (a[i - 1] < b[i - 1]) return -1;
        if (a[i - 1] > b[i - 1]) return 1;
    }
    return 0;
}

static void l5_sub(uint32_t *a, const uint32_t *b, size_t L) {
    int64_t br = 0;
    for (size_t i = 0; i < L; i++) {
        int64_t v = (int64_t)a[i] - (int64_t)b[i] - br;
        if (v < 0) { v += L5_B; br = 1; } else br = 0;
        a[i] = (uint32_t)v;
    }
}

static void l5_mul10(uint32_t *x, size_t L) {
    uint64_t c = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t v = (uint64_t)x[i] * 10 + c;
        x[i] = (uint32_t)(v % L5_B);
        c = v / L5_B;
    }
}

static uint8_t *l5_digits = NULL;
static size_t l5_have = 0;

static void l5_compute(size_t D) {
    size_t K = D + 30;
    size_t N = 5 * K + 60;
    size_t M = 3 * K + 60;
    size_t L = (3 * K + 200) / 9 + 4;
    size_t sz = L * sizeof(uint32_t);
    uint32_t *A0 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *A1 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *A2 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *Fp = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *Fc = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *Fn = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *FN = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *FN1 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *FM = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *FM1 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *P = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *Q = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *T = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint64_t *small = (uint64_t *)calloc(N + 2, sizeof(uint64_t));
    uint8_t *out = (uint8_t *)malloc(D);
    size_t i;

    /* a sequence */
    A0[0] = 1; A1[0] = 2;
    small[0] = 1; small[1] = 2;
    uint64_t bprev = 5;
    size_t jp = 2;
    for (size_t n = 2; n <= N; n++) {
        uint64_t b;
        if (n == 2) b = 5;
        else {
            uint64_t c = bprev + 1;
            while (jp < n && small[jp] == c) { c++; jp++; }
            b = c;
        }
        bprev = b;
        l5_add(A2, A0, A1, L);
        l5_add_small(A2, L, b + n);
        {
            uint64_t sv;
            if (A2[2] || A2[3] || A2[4]) sv = UINT64_MAX;
            else sv = (uint64_t)A2[0] + (uint64_t)A2[1] * L5_B;
            if (A2[2] || A2[3] || A2[4]) sv = UINT64_MAX;
            small[n] = sv;
        }
        { uint32_t *t = A0; A0 = A1; A1 = A2; A2 = t; }
    }
    /* A1 now holds a(N) */

    /* Fibonacci */
    Fp[0] = 0; Fc[0] = 1;
    for (i = 1; i <= N; i++) {
        if (i == N) { memcpy(FN, Fc, sz); memcpy(FN1, Fp, sz); }
        if (i == M) memcpy(FM, Fc, sz);
        if (i == M + 1) memcpy(FM1, Fc, sz);
        l5_add(Fn, Fp, Fc, L);
        { uint32_t *t = Fp; Fp = Fc; Fc = Fn; Fn = t; }
    }

    l5_mul(P, A1, FM, L);
    l5_mul(Q, FN, FM1, L);
    l5_mul(T, FN1, FM, L);
    l5_add(Q, Q, T, L);

    /* long division */
    for (i = 0; i < D; i++) {
        int d = 0;
        while (l5_cmp(P, Q, L) >= 0) { l5_sub(P, Q, L); d++; }
        out[i] = (uint8_t)d;
        l5_mul10(P, L);
    }

    free(A0); free(A1); free(A2); free(Fp); free(Fc); free(Fn);
    free(FN); free(FN1); free(FM); free(FM1); free(P); free(Q); free(T);
    free(small);
    free(l5_digits);
    l5_digits = out;
    l5_have = D;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if ((size_t)n > l5_have) {
        size_t D = l5_have * 2;
        if (D < (size_t)n + 10) D = (size_t)n + 10;
        if (D < 64) D = 64;
        l5_compute(D);
    }
    return (seq_t)l5_digits[n - 1];
}
