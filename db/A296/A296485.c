/*
 * A296485  Decimal expansion of ratio-sum for A295960; see Comments.
 * https://oeis.org/A296485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = floor(10^(n-1) * S) mod 10, where S = Sum_{k>=1}
 *            |A295960(k)/A295960(k-1) - phi|, phi = (1+sqrt(5))/2. A295960:
 *            a(k) = a(k-1) + a(k-2) + b(k) - 1, a(0)=1, a(1)=3, b(0)=2,
 *            b(1)=4, b(2)=5, b(k) = least positive integer not among earlier
 *            a, b. Computed with fixed-point decimal arithmetic on b...
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

static void rs_add3(uint32_t *out, const uint32_t *x, const uint32_t *y, size_t L) {
    uint32_t carry = 0;
    for (size_t i = 0; i < L; i++) {
        uint32_t s = x[i] + y[i] + carry;
        if (s >= RS_BASE) { s -= RS_BASE; carry = 1; } else carry = 0;
        out[i] = s;
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

static void rs_mul10(uint32_t *x, size_t L) {
    uint64_t carry = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t v = (uint64_t)x[i] * 10 + carry;
        x[i] = (uint32_t)(v % RS_BASE);
        carry = v / RS_BASE;
    }
}

/* out[0] = integer part of num/den, out[1..P] = decimal digits (truncated) */
static void rs_div(const uint32_t *num, const uint32_t *den, size_t L, size_t P,
                   uint32_t *r, int *out) {
    memcpy(r, num, L * sizeof(uint32_t));
    int q = 0;
    while (rs_cmp(r, den, L) >= 0) { rs_sub(r, den, L); q++; }
    out[0] = q;
    for (size_t i = 1; i <= P; i++) {
        rs_mul10(r, L);
        int d = 0;
        while (rs_cmp(r, den, L) >= 0) { rs_sub(r, den, L); d++; }
        out[i] = d;
    }
}

/* cmp digit arrays of length P+1 */
static int rs_dcmp(const int *x, const int *y, size_t P) {
    for (size_t i = 0; i <= P; i++) {
        if (x[i] < y[i]) return -1;
        if (x[i] > y[i]) return 1;
    }
    return 0;
}

static uint8_t *rs_digits = NULL;
static size_t rs_have = 0;

static void rs_compute(size_t M) {
    size_t P = M + 30;
    size_t K = 5 * P + 100;
    size_t L = P / 8 + 20;
    uint32_t *u = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *v = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *w = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *r = (uint32_t *)calloc(L, sizeof(uint32_t));
    int *phi = (int *)calloc(P + 1, sizeof(int));
    int *x = (int *)calloc(P + 1, sizeof(int));
    int *sum = (int *)calloc(P + 1, sizeof(int));
    int *dif = (int *)calloc(P + 1, sizeof(int));

    /* phi ~ F(K+1)/F(K) */
    u[0] = 1; v[0] = 1;
    for (size_t j = 1; j < K; j++) {
        rs_add3(w, u, v, L);
        uint32_t *t = u; u = v; v = w; w = t;
    }
    /* u = F(K), v = F(K+1) */
    rs_div(v, u, L, P, r, phi);

    /* A295960 ratios */
    uint32_t *prev = u, *cur = v, *nxt = w;
    memset(prev, 0, L * sizeof(uint32_t));
    memset(cur, 0, L * sizeof(uint32_t));
    prev[0] = 1; cur[0] = 3;
    uint64_t *small = (uint64_t *)malloc(256 * sizeof(uint64_t));
    size_t ns = 0;
    small[ns++] = 1; small[ns++] = 3;
    uint64_t bprev = 4;
    const uint64_t LIM = (uint64_t)1 << 62;

    for (size_t k = 1; k <= K; k++) {
        rs_div(cur, prev, L, P, r, x);
        int c = rs_dcmp(x, phi, P);
        const int *hi = c >= 0 ? x : phi;
        const int *lo = c >= 0 ? phi : x;
        int borrow = 0;
        for (size_t i = P + 1; i > 0; i--) {
            int d = hi[i - 1] - lo[i - 1] - borrow;
            if (d < 0) { d += 10; borrow = 1; } else borrow = 0;
            dif[i - 1] = d;
        }
        int carry = 0;
        for (size_t i = P + 1; i > 0; i--) {
            int s = sum[i - 1] + dif[i - 1] + carry;
            if (s >= 10) { s -= 10; carry = 1; } else carry = 0;
            sum[i - 1] = s;
        }
        /* next b */
        uint64_t cand = bprev + 1;
        for (;;) {
            int found = 0;
            for (size_t j = 0; j < ns; j++) if (small[j] == cand) { found = 1; break; }
            if (!found) break;
            cand++;
        }
        bprev = cand;
        rs_add3(nxt, cur, prev, L);
        rs_add_small(nxt, L, cand - 1);
        if (ns < 256 && nxt[1] == 0 && nxt[2] == 0 && nxt[0] < RS_BASE) {
            uint64_t val = nxt[0];
            if (val < LIM) small[ns++] = val;
        }
        uint32_t *t = prev; prev = cur; cur = nxt; nxt = t;
    }

    uint8_t *out = (uint8_t *)malloc(P + 1);
    for (size_t i = 0; i <= P; i++) out[i] = (uint8_t)sum[i];

    free(u); free(v); free(w); free(r);
    free(phi); free(x); free(sum); free(dif); free(small);
    free(rs_digits);
    rs_digits = out;
    rs_have = M;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if ((size_t)n > rs_have) {
        size_t M = rs_have * 2;
        if (M < (size_t)n + 10) M = (size_t)n + 10;
        if (M < 64) M = 64;
        rs_compute(M);
    }
    return (seq_t)rs_digits[n - 1];
}
