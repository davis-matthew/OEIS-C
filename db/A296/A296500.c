/*
 * A296500  Decimal expansion of limiting power-ratio for A294546; see
 *          Comments.
 * https://oeis.org/A296500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = n-th decimal digit of L = lim_{n->oo} A294546(n)/phi^n,
 *            phi=(1+sqrt(5))/2. With r=phi, u(n)=A294546(n)+A294546(n-1)/r
 *            satisfies u(n)=r*u(n-1)+f(n), f(n)=b(n-1)+n, u(1)=2+1/r, so L =
 *            (u(1)/r + sum_{k>=2} f(k)/r^k) * (phi+2)/5, evaluated by
 *            fixed-point Horner (computed, not looked up)
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

/* ---- digits of golden ratio (from A001622) ---- */
static int phi_cmp(const uint32_t *a, const uint32_t *b, size_t L) {
    for (size_t i = L; i > 0; i--) {
        if (a[i - 1] < b[i - 1]) return -1;
        if (a[i - 1] > b[i - 1]) return 1;
    }
    return 0;
}
#define PHI_BASE 1000000000u
static void phi_mul_small(uint32_t *x, size_t L, uint32_t m) {
    uint64_t carry = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t v = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)(v % PHI_BASE);
        carry = v / PHI_BASE;
    }
}
static void phi_add_small(uint32_t *x, size_t L, uint32_t v) {
    uint64_t carry = v;
    for (size_t i = 0; i < L && carry; i++) {
        uint64_t s = (uint64_t)x[i] + carry;
        x[i] = (uint32_t)(s % PHI_BASE);
        carry = s / PHI_BASE;
    }
}
static void phi_sub(uint32_t *a, const uint32_t *b, size_t L) {
    int64_t borrow = 0;
    for (size_t i = 0; i < L; i++) {
        int64_t v = (int64_t)a[i] - (int64_t)b[i] - borrow;
        if (v < 0) { v += PHI_BASE; borrow = 1; } else borrow = 0;
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
static int phi_digit(size_t n) { /* n >= 1 */
    if (n + 5 > phi_have) {
        size_t M = phi_have * 2;
        if (M < n + 20) M = n + 20;
        if (M < 64) M = 64;
        phi_compute(M);
    }
    return phi_digits[n - 1];
}

/* ---- fixed-point arithmetic, base 10000 limbs, F fractional limbs, 2 integer limbs ---- */
#define LB 10000u

static void fx_norm(uint32_t *a, size_t T) {
    uint64_t c = 0;
    for (size_t i = 0; i < T; i++) {
        uint64_t v = (uint64_t)a[i] + c;
        a[i] = (uint32_t)(v % LB);
        c = v / LB;
    }
}
static void fx_mul(uint32_t *res, const uint32_t *a, const uint32_t *b, size_t T, size_t F) {
    uint64_t *tmp = (uint64_t *)calloc(2 * T + 1, sizeof(uint64_t));
    for (size_t i = 0; i < T; i++) {
        if (!a[i]) continue;
        for (size_t j = 0; j < T; j++)
            tmp[i + j] += (uint64_t)a[i] * b[j];
    }
    uint64_t c = 0;
    for (size_t m = 0; m < 2 * T; m++) {
        uint64_t v = tmp[m] + c;
        tmp[m] = v % LB;
        c = v / LB;
    }
    for (size_t j = 0; j < T; j++) res[j] = (uint32_t)tmp[j + F];
    free(tmp);
}
static void fx_add_small(uint32_t *a, size_t T, size_t F, uint32_t v) {
    a[F] += v;
    fx_norm(a, T);
}
static void fx_add(uint32_t *a, const uint32_t *b, size_t T) {
    for (size_t i = 0; i < T; i++) a[i] += b[i];
    fx_norm(a, T);
}
static void fx_mul_small(uint32_t *a, size_t T, uint32_t m) {
    for (size_t i = 0; i < T; i++) a[i] *= m;
    fx_norm(a, T);
}
static void fx_div_small(uint32_t *a, size_t T, uint32_t d) {
    uint64_t rem = 0;
    for (size_t i = T; i > 0; i--) {
        uint64_t v = rem * LB + a[i - 1];
        a[i - 1] = (uint32_t)(v / d);
        rem = v % d;
    }
}

static char *c_digits = NULL;
static size_t c_have = 0;

static void c_compute(size_t D) {
    size_t F = (D + 30) / 4 + 2, T = F + 2;
    size_t N = (D + 40) * 5;

    /* b(k) by complementary-sequence simulation; a values saturated */
    const uint64_t CAP = 1000000000000000ULL;
    uint64_t *av = (uint64_t *)calloc(N + 3, sizeof(uint64_t));
    uint64_t *bv = (uint64_t *)calloc(N + 3, sizeof(uint64_t));
    av[0] = 1; av[1] = 2; bv[0] = 3;
    for (size_t i = 2; i <= N; i++) {
        uint64_t cand = bv[i - 2] + 1;
        for (;;) {
            int found = 0;
            for (size_t j = 0; j < i; j++)
                if (av[j] == cand) { found = 1; break; }
            if (!found) break;
            cand++;
        }
        bv[i - 1] = cand;
        uint64_t v = av[i - 1] + av[i - 2] + bv[i - 1] + i;
        av[i] = v > CAP ? CAP : v;
    }

    /* x = 1/phi = phi - 1 */
    (void)phi_digit(4 * F + 2);
    uint32_t *x = (uint32_t *)calloc(T, sizeof(uint32_t));
    for (size_t q = 0; q < F; q++) {
        uint32_t v = 0;
        for (size_t i = 0; i < 4; i++)
            v = v * 10 + (uint32_t)phi_digit(4 * q + i + 2);
        x[F - 1 - q] = v;
    }

    uint32_t *H = (uint32_t *)calloc(T, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(T, sizeof(uint32_t));
    uint32_t *x2 = (uint32_t *)calloc(T, sizeof(uint32_t));
    uint32_t *S = (uint32_t *)calloc(T, sizeof(uint32_t));
    for (size_t k = N; k >= 2; k--) {
        fx_mul(t, H, x, T, F);
        memcpy(H, t, T * sizeof(uint32_t));
        fx_add_small(H, T, F, (uint32_t)(bv[k - 1] + k));
    }
    fx_mul(t, H, x, T, F);
    fx_mul(S, t, x, T, F);          /* S = sum f(k) x^k */
    fx_mul(x2, x, x, T, F);         /* x^2 */
    /* Lu = S + x^2 + 2x */
    fx_add(S, x2, T);
    fx_add(S, x, T);
    fx_add(S, x, T);
    /* L = Lu*(x+3)/5 */
    uint32_t *P = (uint32_t *)calloc(T, sizeof(uint32_t));
    fx_mul(P, S, x, T, F);
    fx_mul_small(S, T, 3);
    fx_add(P, S, T);
    fx_div_small(P, T, 5);

    char *out = (char *)malloc(D + 2);
    out[0] = (char)('0' + (P[F] + P[F + 1] * LB) % 10);
    for (size_t d = 1; d < D; d++) {
        size_t q = (d - 1) / 4, pos = (d - 1) % 4;
        uint32_t v = P[F - 1 - q];
        uint32_t div = 1;
        for (size_t i = 0; i < 3 - pos; i++) div *= 10;
        out[d] = (char)('0' + (v / div) % 10);
    }
    free(av); free(bv); free(x); free(H); free(t); free(x2); free(S); free(P);
    free(c_digits);
    c_digits = out;
    c_have = D;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if ((size_t)n > c_have) {
        size_t D = (size_t)n * 2;
        if (D < 100) D = 100;
        c_compute(D);
    }
    return (seq_t)(unsigned)(c_digits[(size_t)n - 1] - '0');
}
