/*
 * A296002  Decimal expansion of the limiting ratio of terms in A296001.
 * https://oeis.org/A296002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, cons
 * Formula:   a(n) = n-th decimal digit of lim_{k->oo} A296001(k)/A296001(k-1)
 *            = 4.302809...; computed with exact bignum terms of A296001
 *            (a(0)=1,a(1)=2,b(0)=3, a(k)=sum_{i<k} a(i)*b(k-1-i)) and long
 *            division of the ratio of two late terms
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static int cmp_big(const uint32_t *x, const uint32_t *y, int L) {
    for (int j = L - 1; j >= 0; --j) {
        if (x[j] != y[j]) return x[j] > y[j] ? 1 : -1;
    }
    return 0;
}

static void sub_big(uint32_t *x, const uint32_t *y, int L) {
    int64_t borrow = 0;
    for (int j = 0; j < L; ++j) {
        int64_t t = (int64_t)x[j] - (int64_t)y[j] - borrow;
        if (t < 0) { t += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        x[j] = (uint32_t)t;
    }
}

static unsigned char *A296002_digits = NULL;
static size_t A296002_have = 0;   /* digits stored: index 0 = integer part */

static void A296002_compute(size_t nd) {
    size_t N = 2 * nd + 100;
    int L = (int)(N * 3 / 32) + 6;
    size_t M = 4 * N + 1000;
    uint32_t *A = (uint32_t *)calloc((N + 1) * (size_t)L, sizeof(uint32_t));
    unsigned char *mark = (unsigned char *)calloc(M + 1, 1);
    uint32_t *b = (uint32_t *)calloc(N + 2, sizeof(uint32_t));
    if (!A || !mark || !b) return;
    A[0] = 1; A[(size_t)L] = 2;
    mark[1] = 1; mark[2] = 1;
    uint32_t cand = 1;
    size_t bl = 0;
    b[bl++] = 3; mark[3] = 1;
    for (size_t k = 2; k <= N; ++k) {
        while (bl <= k - 1) {
            while (mark[cand]) ++cand;
            mark[cand] = 1;
            b[bl++] = cand;
        }
        uint32_t *res = A + k * (size_t)L;
        for (size_t i = 0; i < k; ++i) {
            const uint32_t *ai = A + i * (size_t)L;
            uint64_t bm = b[k - 1 - i];
            uint64_t carry = 0;
            for (int j = 0; j < L; ++j) {
                uint64_t t = (uint64_t)res[j] + (uint64_t)ai[j] * bm + carry;
                res[j] = (uint32_t)t;
                carry = t >> 32;
            }
        }
        bool small = true;
        for (int j = 1; j < L; ++j) if (res[j]) { small = false; break; }
        if (small && res[0] <= M) mark[res[0]] = 1;
    }
    uint32_t *D = A + (N - 1) * (size_t)L;
    uint32_t *R = A + N * (size_t)L;
    unsigned char *dg = (unsigned char *)malloc(nd + 1);
    int q = 0;
    while (cmp_big(R, D, L) >= 0) { sub_big(R, D, L); ++q; }
    dg[0] = (unsigned char)q;
    for (size_t d = 1; d < nd; ++d) {
        uint64_t carry = 0;
        for (int j = 0; j < L; ++j) {
            uint64_t t = (uint64_t)R[j] * 10 + carry;
            R[j] = (uint32_t)t;
            carry = t >> 32;
        }
        int dig = 0;
        while (cmp_big(R, D, L) >= 0) { sub_big(R, D, L); ++dig; }
        dg[d] = (unsigned char)dig;
    }
    free(A); free(mark); free(b);
    free(A296002_digits);
    A296002_digits = dg;
    A296002_have = nd;
}

seq_t a(idx_t n) {
    size_t nd = (size_t)n;
    if (nd < 1) return (seq_t)0;
    if (A296002_have < nd) {
        size_t want = nd < 100 ? 100 : nd + nd / 2;
        A296002_compute(want);
    }
    return (seq_t)A296002_digits[nd - 1];
}
