/*
 * A288238  Decimal expansion of the limiting ratio of consecutive terms of
 *          A288135.
 * https://oeis.org/A288238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = n-th digit of L = 1/x0, where x0 is the smallest positive
 *            root of Sum_{k>=0} floor((k+1)*sqrt(7/3)) * (-x)^k (then L = lim
 *            A288135(n+1)/A288135(n)); computed by fixed-point bisection on
 *            x0 with big-integer limbs, then digit extraction
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static unsigned long long A288238_isqrt(unsigned long long v)
{
    unsigned long long lo = 0, hi = 4294967295ULL;
    while (lo < hi) {
        unsigned long long mid = lo + (hi - lo + 1) / 2;
        if (mid * mid <= v) lo = mid; else hi = mid - 1;
    }
    return lo;
}

static void A288238_mulfix(uint32_t *o, const uint32_t *a, const uint32_t *b, int W, int S)
{
    uint32_t *t = (uint32_t *)calloc((size_t)(2 * W), sizeof(uint32_t));
    int i, j;
    if (!t) exit(EXIT_FAILURE);
    for (i = 0; i < W; i++) {
        uint64_t carry = 0;
        if (a[i] == 0) continue;
        for (j = 0; j < W; j++) {
            uint64_t cur = (uint64_t)a[i] * b[j] + t[i + j] + carry;
            t[i + j] = (uint32_t)cur;
            carry = cur >> 32;
        }
        t[i + W] = (uint32_t)carry;
    }
    for (j = 0; j < W; j++) o[j] = t[S + j];
    free(t);
}

static int A288238_cmp(const uint32_t *a, const uint32_t *b, int W)
{
    int i;
    for (i = W - 1; i >= 0; i--) {
        if (a[i] != b[i]) return a[i] > b[i] ? 1 : -1;
    }
    return 0;
}

/* a -= b (a >= b) */
static void A288238_sub(uint32_t *a, const uint32_t *b, int W)
{
    int64_t borrow = 0;
    int i;
    for (i = 0; i < W; i++) {
        int64_t cur = (int64_t)a[i] - (int64_t)b[i] - borrow;
        if (cur < 0) { cur += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        a[i] = (uint32_t)cur;
    }
}

static void A288238_add(uint32_t *a, const uint32_t *b, int W)
{
    uint64_t carry = 0;
    int i;
    for (i = 0; i < W; i++) {
        uint64_t cur = (uint64_t)a[i] + b[i] + carry;
        a[i] = (uint32_t)cur;
        carry = cur >> 32;
    }
}

static void A288238_shr1(uint32_t *a, int W)
{
    int i;
    for (i = 0; i < W; i++) {
        a[i] = (a[i] >> 1) | (i + 1 < W ? (a[i + 1] << 31) : 0);
    }
}

static unsigned char *A288238_digits = NULL;
static unsigned long long A288238_cap = 0;

static void A288238_compute(unsigned long long D)
{
    int P = (int)(((D + 30) * 10) / 3 + 1 + 64);
    int S, W, K, k, it;
    uint32_t *c, *lo, *hi, *mid, *h, *m, *one, *rem, *X;
    P = (P + 31) / 32 * 32;
    S = P / 32;
    W = S + 2;
    K = P + P / 4 + 40;

    c = (uint32_t *)malloc((size_t)(K + 1) * sizeof(uint32_t));
    lo = (uint32_t *)calloc((size_t)W, 4);
    hi = (uint32_t *)calloc((size_t)W, 4);
    mid = (uint32_t *)calloc((size_t)W, 4);
    h = (uint32_t *)calloc((size_t)W, 4);
    m = (uint32_t *)calloc((size_t)W, 4);
    one = (uint32_t *)calloc((size_t)W, 4);
    rem = (uint32_t *)calloc((size_t)W, 4);
    X = (uint32_t *)calloc((size_t)W, 4);
    if (!c || !lo || !hi || !mid || !h || !m || !one || !rem || !X) exit(EXIT_FAILURE);

    for (k = 0; k <= K; k++) {
        unsigned long long mm = (unsigned long long)k + 1;
        c[k] = (uint32_t)(A288238_isqrt(21ULL * mm * mm) / 3);
    }
    one[S] = 1;
    /* lo = 7/16, hi = 9/16 */
    lo[S - 1] = 0x70000000u;
    hi[S - 1] = 0x90000000u;

    for (it = 0; it < P + 2; it++) {
        int pos;
        memcpy(mid, lo, (size_t)W * 4);
        A288238_add(mid, hi, W);
        A288238_shr1(mid, W);
        /* Horner: h_k = c_k - x*h_{k+1}, k = K..1 */
        memset(h, 0, (size_t)W * 4);
        h[S] = c[K];
        for (k = K - 1; k >= 1; k--) {
            A288238_mulfix(m, mid, h, W, S);
            memset(h, 0, (size_t)W * 4);
            h[S] = c[k];
            A288238_sub(h, m, W);
        }
        A288238_mulfix(m, mid, h, W, S);
        pos = (A288238_cmp(one, m, W) > 0); /* f(mid) > 0 */
        if (pos) memcpy(lo, mid, (size_t)W * 4);
        else memcpy(hi, mid, (size_t)W * 4);
    }
    memcpy(X, lo, (size_t)W * 4);

    memset(rem, 0, (size_t)W * 4);
    rem[S] = 1;
    free(A288238_digits);
    A288238_digits = (unsigned char *)malloc((size_t)D + 2);
    if (!A288238_digits) exit(EXIT_FAILURE);
    {
        unsigned long long i;
        for (i = 1; i <= D; i++) {
            int d = 0, j;
            uint64_t carry = 0;
            while (A288238_cmp(rem, X, W) >= 0) { A288238_sub(rem, X, W); d++; }
            A288238_digits[i] = (unsigned char)d;
            for (j = 0; j < W; j++) {
                uint64_t cur = (uint64_t)rem[j] * 10 + carry;
                rem[j] = (uint32_t)cur;
                carry = cur >> 32;
            }
        }
    }
    A288238_cap = D;
    free(c); free(lo); free(hi); free(mid); free(h); free(m); free(one); free(rem); free(X);
}

seq_t a(idx_t n)
{
    if ((unsigned long long)n > A288238_cap) {
        unsigned long long D = (unsigned long long)n;
        if (D < 128) D = 128;
        A288238_compute(D);
    }
    return (seq_t)A288238_digits[(size_t)n];
}
