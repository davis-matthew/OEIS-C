/*
 * A296486  Decimal expansion of limiting power-ratio for A295960; see
 *          Comments.
 * https://oeis.org/A296486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   limiting power-ratio L = lim a(n)/phi^n for A295960
 *            (a(n)=a(n-1)+a(n-2)+b(n)-1, a(0)=1, a(1)=3, b(0)=2, b(1)=4,
 *            b(2)=5, a and b complementary), with phi=(1+sqrt(5))/2; here L =
 *            (3 + 1/phi + Sum_{k>=2} (b(k)-1)/phi^(k-1)) / sqrt(5).
 *            Fixed-point big arithmetic; a(n) = n-th digit of L (a(1)=5)
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

#define LR_BASE 1000000000u

static int lr_cmp(const uint32_t *a, const uint32_t *b, size_t L) {
    for (size_t i = L; i > 0; i--) {
        if (a[i - 1] < b[i - 1]) return -1;
        if (a[i - 1] > b[i - 1]) return 1;
    }
    return 0;
}
static void lr_mul_small(uint32_t *x, size_t L, uint32_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t v = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)(v % LR_BASE);
        carry = v / LR_BASE;
    }
}
static void lr_add_small(uint32_t *x, size_t L, uint32_t v) {
    uint64_t carry = v;
    for (size_t i = 0; i < L && carry; i++) {
        uint64_t s = (uint64_t)x[i] + carry;
        x[i] = (uint32_t)(s % LR_BASE);
        carry = s / LR_BASE;
    }
}
static void lr_sub(uint32_t *a, const uint32_t *b, size_t L) {
    int64_t borrow = 0;
    for (size_t i = 0; i < L; i++) {
        int64_t v = (int64_t)a[i] - (int64_t)b[i] - borrow;
        if (v < 0) { v += LR_BASE; borrow = 1; } else borrow = 0;
        a[i] = (uint32_t)v;
    }
}
static void lr_div_small(uint32_t *x, size_t n, uint32_t d) {
    uint64_t rem = 0;
    for (size_t i = n; i > 0; i--) {
        uint64_t v = rem * LR_BASE + x[i - 1];
        x[i - 1] = (uint32_t)(v / d);
        rem = v % d;
    }
}

/* digits of sqrt(5): s[0]=2, s[1..]=fraction */
static void lr_sqrt5(uint8_t *s, size_t M) {
    size_t L = M / 9 + 4;
    uint32_t *p = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *r = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *d = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    for (size_t i = 0; i < M; i++) {
        lr_mul_small(r, L, 100);
        lr_add_small(r, L, i == 0 ? 5 : 0);
        memcpy(d, p, L * sizeof(uint32_t));
        lr_mul_small(d, L, 20);
        uint32_t lo = 0, hi = 9;
        while (lo < hi) {
            uint32_t mid = (lo + hi + 1) / 2;
            memcpy(t, d, L * sizeof(uint32_t));
            lr_add_small(t, L, mid);
            lr_mul_small(t, L, mid);
            if (lr_cmp(t, r, L) <= 0) lo = mid; else hi = mid - 1;
        }
        memcpy(t, d, L * sizeof(uint32_t));
        lr_add_small(t, L, lo);
        lr_mul_small(t, L, lo);
        lr_sub(r, t, L);
        lr_mul_small(p, L, 10);
        lr_add_small(p, L, lo);
        s[i] = (uint8_t)lo;
    }
    free(p); free(r); free(d); free(t);
}

static uint8_t *lr_dig = NULL;
static size_t lr_have = 0;

static void lr_compute(size_t D) {
    size_t W = 2 * D + 80;
    size_t L = (W + 8) / 9;
    size_t M = 9 * L + 1;
    size_t n = L + 1;
    size_t K = (D + 20) * 48 / 10 + 10;
    size_t N = K + 200;
    size_t i, k;

    /* f(k) = b(k) - 1 */
    uint32_t *bv = (uint32_t *)calloc(K + 3, sizeof(uint32_t));
    uint8_t *mark = (uint8_t *)calloc(N + 2, 1);
    mark[1] = mark[2] = mark[3] = mark[4] = 1;
    {
        uint64_t a0 = 1, a1 = 3;
        int adone = 0;
        uint64_t cand = 4;
        for (k = 2; k <= K + 1; k++) {
            cand++;
            while (cand <= N && mark[cand]) cand++;
            bv[k] = (uint32_t)cand;
            if (cand <= N) mark[cand] = 1;
            if (!adone) {
                uint64_t a2 = a1 + a0 + cand - 1;
                if (a2 > N) adone = 1;
                else mark[a2] = 1;
                a0 = a1; a1 = a2;
            }
        }
    }

    uint8_t *s = (uint8_t *)malloc(M);
    lr_sqrt5(s, M);
    uint32_t *S5 = (uint32_t *)calloc(n, sizeof(uint32_t));
    uint32_t pw[9];
    pw[0] = 1;
    for (i = 1; i < 9; i++) pw[i] = pw[i - 1] * 10;
    S5[L] = s[0];
    for (i = 1; i < M; i++) {
        size_t fi = i - 1;
        S5[L - 1 - fi / 9] += (uint32_t)s[i] * pw[8 - fi % 9];
    }
    free(s);

    uint32_t *x = (uint32_t *)malloc(n * sizeof(uint32_t));
    memcpy(x, S5, n * sizeof(uint32_t));
    x[L] -= 1;
    lr_div_small(x, n, 2);

    uint32_t *prev = (uint32_t *)calloc(n, sizeof(uint32_t));
    uint32_t *cur = (uint32_t *)malloc(n * sizeof(uint32_t));
    uint32_t *nxt = (uint32_t *)calloc(n, sizeof(uint32_t));
    prev[L] = 1;
    memcpy(cur, x, n * sizeof(uint32_t));
    uint64_t *acc = (uint64_t *)calloc(n, sizeof(uint64_t));

    for (k = 2; k <= K + 1; k++) {
        uint64_t f = (uint64_t)bv[k] - 1;
        for (i = 0; i < n; i++) acc[i] += f * cur[i];
        memcpy(nxt, prev, n * sizeof(uint32_t));
        lr_sub(nxt, cur, n);
        uint32_t *tmp = prev; prev = cur; cur = nxt; nxt = tmp;
    }

    uint32_t *T = (uint32_t *)calloc(n, sizeof(uint32_t));
    {
        uint64_t carry = 0;
        for (i = 0; i < n; i++) {
            uint64_t v = acc[i] + carry + x[i];
            if (i == L) { T[i] = (uint32_t)v; carry = 0; }
            else { T[i] = (uint32_t)(v % LR_BASE); carry = v / LR_BASE; }
        }
        T[L] += 3;
    }

    uint32_t *res = (uint32_t *)calloc(2 * n + 2, sizeof(uint32_t));
    for (i = 0; i < n; i++) {
        if (T[i] == 0) continue;
        uint64_t carry = 0;
        for (size_t j = 0; j < n; j++) {
            uint64_t v = (uint64_t)res[i + j] + (uint64_t)T[i] * S5[j] + carry;
            res[i + j] = (uint32_t)(v % LR_BASE);
            carry = v / LR_BASE;
        }
        res[i + n] = (uint32_t)carry;
    }
    uint32_t *R = (uint32_t *)malloc(n * sizeof(uint32_t));
    memcpy(R, res + L, n * sizeof(uint32_t));
    lr_div_small(R, n, 5);

    uint8_t *out = (uint8_t *)malloc(D + 1);
    out[0] = (uint8_t)(R[L] % 10);
    for (i = 1; i < D; i++) {
        size_t fi = i - 1;
        out[i] = (uint8_t)((R[L - 1 - fi / 9] / pw[8 - fi % 9]) % 10);
    }

    free(bv); free(mark); free(S5); free(x); free(prev); free(cur); free(nxt);
    free(acc); free(T); free(res); free(R);
    free(lr_dig);
    lr_dig = out;
    lr_have = D;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if ((size_t)n > lr_have) {
        size_t D = lr_have * 2;
        if (D < (size_t)n + 30) D = (size_t)n + 30;
        if (D < 64) D = 64;
        lr_compute(D);
    }
    return (seq_t)lr_dig[n - 1];
}
