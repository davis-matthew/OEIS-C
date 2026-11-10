/*
 * A296479  Decimal expansion of ratio-sum for A295951; see Comments.
 * https://oeis.org/A296479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = n-th decimal digit of S = Sum_{k>=1} (A(k)/A(k-1) - phi),
 *            where A = A295951 (A(0)=2, A(1)=3, A(k) = A(k-1) + A(k-2) +
 *            b(k), with b the complement of A and b(2)=5) and phi =
 *            (1+sqrt(5))/2. The OEIS constant 2.5719... is this signed sum:
 *            only the k=1 term is negative (3/2 < phi). The sum is...
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
#include <stdbool.h>

#define RS_BASE 1000000000u
#define RS_M 65536

static unsigned char *rs_digits = NULL;
static size_t rs_have = 0;

static int rs_cmp(const uint32_t *x, const uint32_t *y, size_t L)
{
    for (size_t i = L; i > 0; i--) {
        if (x[i-1] < y[i-1]) return -1;
        if (x[i-1] > y[i-1]) return 1;
    }
    return 0;
}

static void rs_sub(uint32_t *x, const uint32_t *y, size_t L)
{
    int64_t borrow = 0;
    for (size_t i = 0; i < L; i++) {
        int64_t v = (int64_t)x[i] - (int64_t)y[i] - borrow;
        if (v < 0) { v += RS_BASE; borrow = 1; } else borrow = 0;
        x[i] = (uint32_t)v;
    }
}

static void rs_mul10(uint32_t *x, size_t L)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t v = (uint64_t)x[i] * 10u + carry;
        x[i] = (uint32_t)(v % RS_BASE);
        carry = v / RS_BASE;
    }
}

/* digits[0] = integer part of num/den, digits[1..P] = decimals (truncated) */
static void rs_ratio(const uint32_t *num, const uint32_t *den, size_t L,
                     size_t P, unsigned char *digits, uint32_t *r)
{
    memcpy(r, num, L * sizeof(uint32_t));
    for (size_t i = 0; i <= P; i++) {
        if (i > 0) rs_mul10(r, L);
        unsigned c = 0;
        while (c < 9 && rs_cmp(r, den, L) >= 0) {
            rs_sub(r, den, L);
            c++;
        }
        digits[i] = (unsigned char)c;
    }
}

static void rs_add3(uint32_t *out, const uint32_t *x, const uint32_t *y,
                    uint32_t small, size_t L)
{
    uint64_t carry = small;
    for (size_t i = 0; i < L; i++) {
        uint64_t v = (uint64_t)x[i] + y[i] + carry;
        out[i] = (uint32_t)(v % RS_BASE);
        carry = v / RS_BASE;
    }
}

static void rs_compute(size_t P)
{
    size_t K = 5 * P + 40;
    size_t L = (P * 11 / 10 + 30) / 9 + 3;
    uint32_t *A = (uint32_t *)calloc((K + 1) * L, sizeof(uint32_t));
    unsigned char *mark = (unsigned char *)calloc(RS_M, 1);
    uint32_t *r = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *f0 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *f1 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *f2 = (uint32_t *)calloc(L, sizeof(uint32_t));
    unsigned char *phi = (unsigned char *)calloc(P + 1, 1);
    unsigned char *x = (unsigned char *)calloc(P + 1, 1);
    long long *acc = (long long *)calloc(P + 1, sizeof(long long));
    unsigned char *out = (unsigned char *)calloc(P + 1, 1);

    A[0] = 2; A[L] = 3;
    mark[2] = 1; mark[3] = 1;
    uint32_t next_c = 5;
    for (size_t k = 2; k <= K; k++) {
        uint32_t cand = next_c;
        while (cand < RS_M - 1 && mark[cand]) cand++;
        next_c = cand + 1;
        rs_add3(A + k * L, A + (k-1) * L, A + (k-2) * L, cand, L);
        const uint32_t *v = A + k * L;
        bool small = true;
        for (size_t i = 1; i < L; i++) if (v[i]) { small = false; break; }
        if (small && v[0] < RS_M) mark[v[0]] = 1;
    }

    /* phi ~ F(K+1)/F(K) */
    f0[0] = 1; f1[0] = 1;
    for (size_t k = 2; k <= K; k++) {
        rs_add3(f2, f0, f1, 0, L);
        memcpy(f0, f1, L * sizeof(uint32_t));
        memcpy(f1, f2, L * sizeof(uint32_t));
    }
    rs_ratio(f1, f0, L, P, phi, r);

    /* signed sum of (ratio - phi), digit-wise */
    for (size_t k = 1; k <= K; k++) {
        rs_ratio(A + k * L, A + (k-1) * L, L, P, x, r);
        for (size_t i = 0; i <= P; i++)
            acc[i] += (long long)x[i] - (long long)phi[i];
    }
    long long carry = 0;
    for (size_t i = P + 1; i > 1; i--) {
        long long v = acc[i-1] + carry;
        long long d = ((v % 10) + 10) % 10;
        out[i-1] = (unsigned char)d;
        carry = (v - d) / 10;
    }
    {
        long long v = acc[0] + carry;
        if (v < 0) v = 0;
        out[0] = (unsigned char)(v % 10);
    }

    free(A); free(mark); free(r); free(f0); free(f1); free(f2);
    free(phi); free(x); free(acc);
    free(rs_digits);
    rs_digits = out;
    rs_have = P + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t need = (size_t)n + 25;
    if (rs_have < need) {
        size_t P = need;
        if (P < 80) P = 80;
        rs_compute(P);
        rs_have = P - 20; /* digits reliably correct up to here */
    }
    return (seq_t)rs_digits[(size_t)n - 1];
}
