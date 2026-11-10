/*
 * A296501  Decimal expansion of ratio-sum for A294552; see Comments.
 * https://oeis.org/A296501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = n-th decimal digit of Sum_{k>=1} |A294552(k)/A294552(k-1)
 *            - phi|; A294552(n)=A294552(n-1)+A294552(n-2)+b(n-1)+b(n-2)+n,
 *            A294552(0)=1, A294552(1)=2, b(0)=3, b = complement of A294552
 *            (computed with big integers, digit-by-digit division)
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

static uint8_t *rs_digits = NULL;
static size_t rs_have = 0;

static int rs_cmp(const uint32_t *x, const uint32_t *y, size_t L) {
    for (size_t i = L; i > 0; i--) {
        if (x[i - 1] < y[i - 1]) return -1;
        if (x[i - 1] > y[i - 1]) return 1;
    }
    return 0;
}

static void rs_sub(uint32_t *x, const uint32_t *y, size_t L) {
    int64_t borrow = 0;
    for (size_t i = 0; i < L; i++) {
        int64_t v = (int64_t)x[i] - (int64_t)y[i] - borrow;
        if (v < 0) { v += RS_BASE; borrow = 1; } else borrow = 0;
        x[i] = (uint32_t)v;
    }
}

static void rs_mul10(uint32_t *x, size_t L) {
    uint64_t carry = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t v = (uint64_t)x[i] * 10 + carry;
        x[i] = (uint32_t)(v % RS_BASE);
        carry = v / RS_BASE;
    }
}

static void rs_add_small(uint32_t *x, size_t L, uint64_t v) {
    uint64_t carry = v;
    for (size_t i = 0; i < L && carry; i++) {
        uint64_t s = (uint64_t)x[i] + carry;
        x[i] = (uint32_t)(s % RS_BASE);
        carry = s / RS_BASE;
    }
}

/* r = x + y */
static void rs_add(uint32_t *r, const uint32_t *x, const uint32_t *y, size_t L) {
    uint64_t carry = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t s = (uint64_t)x[i] + y[i] + carry;
        if (s >= RS_BASE) { s -= RS_BASE; carry = 1; } else carry = 0;
        r[i] = (uint32_t)s;
    }
}

static size_t rs_len(const uint32_t *x, size_t L) {
    while (L > 0 && x[L - 1] == 0) L--;
    return L;
}

/* K decimal digits of A/B (A/B < 10): out[0] integer digit, rest fraction */
static void rs_div(const uint32_t *A, const uint32_t *B, uint32_t *rem,
                   uint8_t *out, size_t K, size_t L) {
    memcpy(rem, A, L * sizeof(uint32_t));
    for (size_t j = 0; j < K; j++) {
        if (j > 0) rs_mul10(rem, L);
        uint8_t d = 0;
        while (rs_cmp(rem, B, L) >= 0) { rs_sub(rem, B, L); d++; }
        out[j] = d;
    }
}

static int rs_fx_cmp(const uint8_t *x, const uint8_t *y, size_t K) {
    for (size_t i = 0; i < K; i++) {
        if (x[i] < y[i]) return -1;
        if (x[i] > y[i]) return 1;
    }
    return 0;
}

/* r = x - y, x >= y */
static void rs_fx_sub(uint8_t *r, const uint8_t *x, const uint8_t *y, size_t K) {
    int borrow = 0;
    for (size_t i = K; i > 0; i--) {
        int v = (int)x[i - 1] - (int)y[i - 1] - borrow;
        if (v < 0) { v += 10; borrow = 1; } else borrow = 0;
        r[i - 1] = (uint8_t)v;
    }
}

static void rs_fx_add(uint8_t *x, const uint8_t *y, size_t K) {
    int carry = 0;
    for (size_t i = K; i > 0; i--) {
        int v = (int)x[i - 1] + (int)y[i - 1] + carry;
        if (v >= 10) { v -= 10; carry = 1; } else carry = 0;
        x[i - 1] = (uint8_t)v;
    }
}

static void rs_compute(size_t M) {
    size_t K = M + 16;
    size_t L = (K + 10) / 9 + 12;
    size_t stop = (K + 10) / 9 + 2;
    uint32_t *prev = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *cur = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *nxt = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *rem = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint8_t *phi = (uint8_t *)calloc(K, 1);
    uint8_t *r = (uint8_t *)calloc(K, 1);
    uint8_t *df = (uint8_t *)calloc(K, 1);
    uint8_t *sum = (uint8_t *)calloc(K, 1);
    uint64_t smalls[256];
    size_t ns = 0;

    /* phi from Fibonacci ratio */
    prev[0] = 1; cur[0] = 1;
    while (rs_len(prev, L) <= K / 18 + 3) {
        rs_add(nxt, cur, prev, L);
        memcpy(prev, cur, L * sizeof(uint32_t));
        memcpy(cur, nxt, L * sizeof(uint32_t));
    }
    rs_div(cur, prev, rem, phi, K, L);

    /* A294552 recurrence */
    memset(prev, 0, L * sizeof(uint32_t));
    memset(cur, 0, L * sizeof(uint32_t));
    prev[0] = 1; cur[0] = 2;
    smalls[ns++] = 1; smalls[ns++] = 2;
    uint64_t bold = 3, cand = 4;
    for (uint64_t n = 1;; n++) {
        if (rs_len(prev, L) > stop) break;
        rs_div(cur, prev, rem, r, K, L);
        if (rs_fx_cmp(r, phi, K) >= 0) rs_fx_sub(df, r, phi, K);
        else rs_fx_sub(df, phi, r, K);
        rs_fx_add(sum, df, K);

        for (;;) {
            int used = 0;
            for (size_t j = 0; j < ns; j++) if (smalls[j] == cand) { used = 1; break; }
            if (!used) break;
            cand++;
        }
        uint64_t bnew = cand;
        cand++;
        rs_add(nxt, cur, prev, L);
        rs_add_small(nxt, L, bnew + bold + n + 1);
        memcpy(prev, cur, L * sizeof(uint32_t));
        memcpy(cur, nxt, L * sizeof(uint32_t));
        bold = bnew;
        if (rs_len(cur, L) <= 2 && ns < 256) {
            uint64_t v = (uint64_t)cur[0] + (uint64_t)cur[1] * RS_BASE;
            smalls[ns++] = v;
        }
    }

    uint8_t *out = (uint8_t *)malloc(M);
    memcpy(out, sum, M);
    free(prev); free(cur); free(nxt); free(rem);
    free(phi); free(r); free(df); free(sum);
    free(rs_digits);
    rs_digits = out;
    rs_have = M;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if ((size_t)n > rs_have) {
        size_t M = rs_have * 2;
        if (M < (size_t)n + 20) M = (size_t)n + 20;
        if (M < 64) M = 64;
        rs_compute(M);
    }
    return (seq_t)rs_digits[n - 1];
}
