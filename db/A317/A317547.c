/*
 * A317547  Decimal expansion of f(0) such that f(n) = log(abs(f(n-1))) > 0
 *          iff n is a prime number (negated).
 * https://oeis.org/A317547
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   f(0) = -exp(f(1)), f(k-1) = s(k-1)*exp(f(k)) with s(m)=+1 if m
 *            is prime else -1 (i.e. f(n)=log|f(n-1)|, f(n)>0 iff n prime);
 *            a(n) = digit number n+3 after the decimal point of |f(0)| =
 *            0.0217226898..., computed by multi-limb fixed-point iteration
 *            down from a deep tail
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 100 terms through n = 98 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static unsigned char *A317547_dig = NULL;
static int A317547_nd = 0;

static bool A317547_isp(long long m) {
    long long d;
    if (m < 2) return false;
    for (d = 2; d * d <= m; d++)
        if (m % d == 0) return false;
    return true;
}

/* r = a*b fixed point, W fractional limbs, little-endian, W+1 limbs (top = integer) */
static void A317547_mul(uint32_t *r, const uint32_t *a, const uint32_t *b, int W, uint32_t *tmp) {
    int L = W + 1, i, j;
    memset(tmp, 0, sizeof(uint32_t) * (size_t)(2 * L));
    for (i = 0; i < L; i++) {
        uint64_t carry = 0;
        if (a[i] == 0) continue;
        for (j = 0; j < L; j++) {
            uint64_t cur = (uint64_t)a[i] * b[j] + tmp[i + j] + carry;
            tmp[i + j] = (uint32_t)cur;
            carry = cur >> 32;
        }
        tmp[i + L] += (uint32_t)carry;
    }
    for (i = 0; i < L; i++) r[i] = tmp[i + W];
}

static void A317547_divs(uint32_t *a, uint32_t d, int L) {
    uint64_t rem = 0;
    int i;
    for (i = L - 1; i >= 0; i--) {
        uint64_t cur = (rem << 32) | a[i];
        a[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static void A317547_add(uint32_t *a, const uint32_t *b, int L) {
    uint64_t c = 0;
    int i;
    for (i = 0; i < L; i++) {
        uint64_t cur = (uint64_t)a[i] + b[i] + c;
        a[i] = (uint32_t)cur;
        c = cur >> 32;
    }
}

static void A317547_sub(uint32_t *a, const uint32_t *b, int L) {
    int64_t br = 0;
    int i;
    for (i = 0; i < L; i++) {
        int64_t cur = (int64_t)a[i] - (int64_t)b[i] - br;
        if (cur < 0) { cur += ((int64_t)1 << 32); br = 1; } else br = 0;
        a[i] = (uint32_t)cur;
    }
}

static bool A317547_zero(const uint32_t *a, int L) {
    int i;
    for (i = 0; i < L; i++) if (a[i]) return false;
    return true;
}

static void A317547_compute(int T) {
    int Dg = T + 25;
    int W = (int)((Dg * 3.33) / 32.0) + 5;
    int L = W + 1;
    long long N = 6LL * Dg + 200, k;
    uint32_t *f, *P, *Q, *term, *tmp, *res;
    bool fneg = false;
    int i, pos;
    f = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    P = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    Q = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    term = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    res = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    tmp = (uint32_t *)calloc((size_t)(2 * L), sizeof(uint32_t));
    for (k = N; k >= 1; k--) {
        uint32_t j;
        memset(P, 0, sizeof(uint32_t) * (size_t)L);
        memset(Q, 0, sizeof(uint32_t) * (size_t)L);
        memset(term, 0, sizeof(uint32_t) * (size_t)L);
        term[W] = 1;
        P[W] = 1;
        for (j = 1;; j++) {
            A317547_mul(term, term, f, W, tmp);
            A317547_divs(term, j, L);
            if (A317547_zero(term, L)) break;
            if (!fneg || (j % 2 == 0)) A317547_add(P, term, L);
            else A317547_add(Q, term, L);
        }
        A317547_sub(P, Q, L);
        memcpy(f, P, sizeof(uint32_t) * (size_t)L);
        fneg = !A317547_isp(k - 1);
    }
    memcpy(res, f, sizeof(uint32_t) * (size_t)L);
    free(A317547_dig);
    A317547_dig = (unsigned char *)malloc((size_t)T + 2);
    A317547_dig[0] = 0;
    res[W] = 0;
    for (pos = 1; pos <= T; pos++) {
        uint64_t carry = 0;
        for (i = 0; i <= W; i++) {
            uint64_t cur = (uint64_t)res[i] * 10 + carry;
            res[i] = (uint32_t)cur;
            carry = cur >> 32;
        }
        A317547_dig[pos] = (unsigned char)res[W];
        res[W] = 0;
    }
    A317547_nd = T;
    free(f); free(P); free(Q); free(term); free(res); free(tmp);
}

seq_t a(idx_t n) {
    int pos = (int)n + 3;
    if (pos > A317547_nd) {
        int T = pos;
        if (T < 2 * A317547_nd) T = 2 * A317547_nd;
        if (T < 64) T = 64;
        A317547_compute(T);
    }
    return (seq_t)A317547_dig[pos];
}
