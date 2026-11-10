/*
 * A296489  Decimal expansion of ratio-sum for A293358; see Comments.
 * https://oeis.org/A296489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   ratio-sum = Sum_{n>=1} |a(n)/a(n-1) - phi| for A293358, where
 *            a(n) = a(n-1) + a(n-2) + b(n-1), a(0) = 1, a(1) = 3, b(0) = 2,
 *            b(1) = 4, and b is the complement of a. The terms fall off like
 *            n/phi^n, so the sum needs about 5*P terms for P correct digits.
 *            Phi = F(K+1)/F(K) with K large. All values a...
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

#define RS_BASE 1000000000u

static int rs_cmp(const uint32_t *a, const uint32_t *b, size_t L) {
    for (size_t i = L; i > 0; i--) {
        if (a[i - 1] < b[i - 1]) return -1;
        if (a[i - 1] > b[i - 1]) return 1;
    }
    return 0;
}
static void rs_mul_small(uint32_t *x, size_t L, uint32_t m) {
    uint64_t c = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t v = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)(v % RS_BASE);
        c = v / RS_BASE;
    }
}
static void rs_add_small(uint32_t *x, size_t L, uint32_t v) {
    uint64_t c = v;
    for (size_t i = 0; i < L && c; i++) {
        uint64_t s = (uint64_t)x[i] + c;
        x[i] = (uint32_t)(s % RS_BASE);
        c = s / RS_BASE;
    }
}
static void rs_add(uint32_t *a, const uint32_t *b, size_t L) {
    uint32_t c = 0;
    for (size_t i = 0; i < L; i++) {
        uint32_t s = a[i] + b[i] + c;
        if (s >= RS_BASE) { s -= RS_BASE; c = 1; } else c = 0;
        a[i] = s;
    }
}
static void rs_sub(uint32_t *a, const uint32_t *b, size_t L) {
    int64_t br = 0;
    for (size_t i = 0; i < L; i++) {
        int64_t v = (int64_t)a[i] - (int64_t)b[i] - br;
        if (v < 0) { v += RS_BASE; br = 1; } else br = 0;
        a[i] = (uint32_t)v;
    }
}

/* x = floor(num/den * 10^P), assuming num/den < 10 */
static void rs_divide(const uint32_t *num, const uint32_t *den, size_t L, size_t P,
                      uint32_t *R, uint32_t *x) {
    memcpy(R, num, L * sizeof(uint32_t));
    memset(x, 0, L * sizeof(uint32_t));
    uint32_t q = 0;
    while (rs_cmp(R, den, L) >= 0) { rs_sub(R, den, L); q++; }
    rs_add_small(x, L, q);
    for (size_t i = 0; i < P; i++) {
        rs_mul_small(R, L, 10);
        q = 0;
        while (rs_cmp(R, den, L) >= 0) { rs_sub(R, den, L); q++; }
        rs_mul_small(x, L, 10);
        rs_add_small(x, L, q);
    }
}

static uint8_t *rs_digits = NULL;
static size_t rs_have = 0;

#define RS_USED 1048576u

static void rs_compute(size_t M) {
    size_t P = M + 15;
    size_t T = 5 * P + 60;          /* phi^-T < 10^-(P+10) roughly */
    size_t K = 3 * P + 20;          /* phi error ~ phi^-2K */
    size_t L = (P + T / 4 + K / 4 + 40) / 9 + 8;
    size_t sz = L * sizeof(uint32_t);
    uint32_t *R = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *x = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *F = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *sum = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *f0 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *f1 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *A = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *B = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *C = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)calloc(L, sizeof(uint32_t));
    unsigned char *used = (unsigned char *)calloc(RS_USED, 1);
    uint8_t *out = (uint8_t *)malloc(M);

    f0[0] = 1; f1[0] = 1;
    for (size_t k = 1; k < K; k++) {
        memcpy(tmp, f1, sz);
        rs_add(f1, f0, L);
        memcpy(f0, tmp, sz);
    }
    rs_divide(f1, f0, L, P, R, F);

    A[0] = 1; B[0] = 3;
    used[1] = used[2] = used[3] = used[4] = 1;
    uint32_t nextu = 5, bn = 4;
    for (size_t n = 1; n <= T; n++) {
        rs_divide(B, A, L, P, R, x);
        if (rs_cmp(x, F, L) >= 0) { rs_sub(x, F, L); rs_add(sum, x, L); }
        else { memcpy(tmp, F, sz); rs_sub(tmp, x, L); rs_add(sum, tmp, L); }
        memcpy(C, B, sz);
        rs_add(C, A, L);
        rs_add_small(C, L, bn);
        while (nextu < RS_USED && used[nextu]) nextu++;
        uint32_t nb = nextu;
        int small = 1;
        for (size_t i = 1; i < L; i++) if (C[i]) { small = 0; break; }
        if (small && C[0] < RS_USED) used[C[0]] = 1;
        if (nb < RS_USED) used[nb] = 1;
        bn = nb;
        memcpy(A, B, sz);
        memcpy(B, C, sz);
    }

    for (size_t n = 1; n <= M; n++) {
        size_t pos = P - (n - 1);
        uint32_t limb = sum[pos / 9];
        for (size_t j = 0; j < pos % 9; j++) limb /= 10;
        out[n - 1] = (uint8_t)(limb % 10);
    }

    free(R); free(x); free(F); free(sum); free(f0); free(f1);
    free(A); free(B); free(C); free(tmp); free(used);
    free(rs_digits);
    rs_digits = out;
    rs_have = M;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if ((size_t)n > rs_have) {
        size_t M = rs_have * 2;
        if (M < (size_t)n + 10) M = (size_t)n + 10;
        if (M < 100) M = 100;
        rs_compute(M);
    }
    return (seq_t)rs_digits[n - 1];
}
