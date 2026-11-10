/*
 * A242724  Decimal expansion of a constant associated with self-generating
 *          continued fractions and Cahen's constant.
 * https://oeis.org/A242724
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = n-th decimal digit of Sum_{k>=0} (-1)^k/(s(k)*s(k+1)), s
 *            = A006277: s(k+1) = (s(k)+1)*s(k-1), s(0) = s(1) = 1. Partial
 *            sums are kept as exact big rationals. The digits are fixed once
 *            two consecutive partial sums (which bracket the constant) agree
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Validated: partial, pass, 100 terms through n = 99 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define A242724_BASE 1000000000u

typedef struct { uint32_t *v; size_t n; size_t cap; } A242724_bn;

static A242724_bn A242724_bn_new(size_t cap) {
    A242724_bn b;
    b.cap = cap ? cap : 1;
    b.v = (uint32_t *)calloc(b.cap, sizeof(uint32_t));
    b.n = 0;
    return b;
}
static void A242724_bn_free(A242724_bn *b) { free(b->v); b->v = NULL; b->n = b->cap = 0; }
static void A242724_bn_reserve(A242724_bn *b, size_t c) {
    if (c > b->cap) {
        b->v = (uint32_t *)realloc(b->v, c * sizeof(uint32_t));
        memset(b->v + b->cap, 0, (c - b->cap) * sizeof(uint32_t));
        b->cap = c;
    }
}
static void A242724_bn_trim(A242724_bn *b) { while (b->n && b->v[b->n - 1] == 0) b->n--; }
static A242724_bn A242724_bn_small(uint32_t x) {
    A242724_bn b = A242724_bn_new(1);
    if (x) { b.v[0] = x; b.n = 1; }
    return b;
}
static A242724_bn A242724_bn_copy(const A242724_bn *a) {
    A242724_bn r = A242724_bn_new(a->n + 1);
    if (a->n) memcpy(r.v, a->v, a->n * sizeof(uint32_t));
    r.n = a->n;
    return r;
}
static A242724_bn A242724_bn_mul(const A242724_bn *a, const A242724_bn *b) {
    A242724_bn r = A242724_bn_new(a->n + b->n + 1);
    if (!a->n || !b->n) return r;
    for (size_t i = 0; i < a->n; i++) {
        uint64_t carry = 0, ai = a->v[i];
        for (size_t j = 0; j < b->n; j++) {
            uint64_t cur = (uint64_t)r.v[i + j] + ai * b->v[j] + carry;
            r.v[i + j] = (uint32_t)(cur % A242724_BASE);
            carry = cur / A242724_BASE;
        }
        r.v[i + b->n] = (uint32_t)carry;
    }
    r.n = a->n + b->n;
    A242724_bn_trim(&r);
    return r;
}
static int A242724_bn_cmp(const A242724_bn *a, const A242724_bn *b) {
    if (a->n != b->n) return a->n < b->n ? -1 : 1;
    for (size_t i = a->n; i-- > 0;)
        if (a->v[i] != b->v[i]) return a->v[i] < b->v[i] ? -1 : 1;
    return 0;
}
static void A242724_bn_add_to(A242724_bn *a, const A242724_bn *b) {
    size_t m = (a->n > b->n ? a->n : b->n) + 1;
    A242724_bn_reserve(a, m);
    for (size_t i = a->n; i < m; i++) a->v[i] = 0;
    uint32_t carry = 0;
    for (size_t i = 0; i < m; i++) {
        uint32_t cur = a->v[i] + (i < b->n ? b->v[i] : 0) + carry;
        if (cur >= A242724_BASE) { cur -= A242724_BASE; carry = 1; } else carry = 0;
        a->v[i] = cur;
    }
    a->n = m;
    A242724_bn_trim(a);
}
static void A242724_bn_sub_from(A242724_bn *a, const A242724_bn *b) { /* a >= b */
    int64_t borrow = 0;
    for (size_t i = 0; i < a->n; i++) {
        int64_t cur = (int64_t)a->v[i] - (i < b->n ? (int64_t)b->v[i] : 0) - borrow;
        if (cur < 0) { cur += A242724_BASE; borrow = 1; } else borrow = 0;
        a->v[i] = (uint32_t)cur;
    }
    A242724_bn_trim(a);
}
static void A242724_bn_mul_small(A242724_bn *a, uint32_t m) {
    A242724_bn_reserve(a, a->n + 1);
    uint64_t carry = 0;
    for (size_t i = 0; i < a->n; i++) {
        uint64_t cur = (uint64_t)a->v[i] * m + carry;
        a->v[i] = (uint32_t)(cur % A242724_BASE);
        carry = cur / A242724_BASE;
    }
    if (carry) a->v[a->n++] = (uint32_t)carry;
}

/* first N digits after the decimal point of P/Q */
static void A242724_digits(const A242724_bn *P, const A242724_bn *Q, size_t N, uint8_t *out) {
    A242724_bn r = A242724_bn_copy(P);
    while (A242724_bn_cmp(&r, Q) >= 0) A242724_bn_sub_from(&r, Q);
    for (size_t i = 0; i < N; i++) {
        A242724_bn_mul_small(&r, 10);
        uint8_t d = 0;
        while (A242724_bn_cmp(&r, Q) >= 0) { A242724_bn_sub_from(&r, Q); d++; }
        out[i] = d;
    }
    A242724_bn_free(&r);
}

static uint8_t *A242724_cache = NULL;
static size_t A242724_cache_n = 0;

static void A242724_compute(size_t N) {
    A242724_bn s0 = A242724_bn_small(1), s1 = A242724_bn_small(1);
    A242724_bn P = A242724_bn_small(0), Q = A242724_bn_small(1);
    A242724_bn Pp = A242724_bn_small(0), Qp = A242724_bn_small(1);
    uint8_t *d1 = (uint8_t *)malloc(N), *d2 = (uint8_t *)malloc(N);
    for (size_t k = 0;; k++) {
        A242724_bn d = A242724_bn_mul(&s0, &s1);
        A242724_bn_free(&Pp); A242724_bn_free(&Qp);
        Pp = A242724_bn_copy(&P); Qp = A242724_bn_copy(&Q);
        A242724_bn T = A242724_bn_mul(&P, &d);
        if (k & 1) A242724_bn_sub_from(&T, &Q); else A242724_bn_add_to(&T, &Q);
        A242724_bn Qn = A242724_bn_mul(&Q, &d);
        A242724_bn_free(&P); A242724_bn_free(&Q);
        P = T; Q = Qn;
        bool_check:
        if (k >= 1 && d.n >= 1 && (d.n - 1) * 9 >= N + 1) {
            A242724_digits(&Pp, &Qp, N, d1);
            A242724_digits(&P, &Q, N, d2);
            if (memcmp(d1, d2, N) == 0) { A242724_bn_free(&d); break; }
        }
        A242724_bn_free(&d);
        /* advance s: s(k+2) = (s(k+1)+1)*s(k) */
        A242724_bn one = A242724_bn_small(1);
        A242724_bn s1p = A242724_bn_copy(&s1);
        A242724_bn_add_to(&s1p, &one);
        A242724_bn nx = A242724_bn_mul(&s1p, &s0);
        A242724_bn_free(&one); A242724_bn_free(&s1p);
        A242724_bn_free(&s0);
        s0 = s1; s1 = nx;
    }
    free(A242724_cache);
    A242724_cache = d2;
    A242724_cache_n = N;
    free(d1);
    A242724_bn_free(&s0); A242724_bn_free(&s1);
    A242724_bn_free(&P); A242724_bn_free(&Q);
    A242724_bn_free(&Pp); A242724_bn_free(&Qp);
}

seq_t a(idx_t n) {
    if ((size_t)n >= A242724_cache_n) {
        size_t N = (size_t)n + 1;
        if (N < 2 * A242724_cache_n) N = 2 * A242724_cache_n;
        if (N < 64) N = 64;
        A242724_compute(N);
    }
    return (seq_t)A242724_cache[n];
}
