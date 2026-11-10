/*
 * A293736  Number of multisets of nonempty words with a total of n letters
 *          over senary alphabet such that within each prefix of a word every
 *          letter of the alphabet is at least as frequent as the subsequent
 *          alphabet letter.
 * https://oeis.org/A293736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: Product_{j>=1} 1/(1-x^j)^A007579(j), computed with the
 *            Euler transform a(0) = 1, a(n) = (1/n) * Sum_{k=1..n}
 *            c(k)*a(n-k), where c(k) = Sum_{d|k} d*A007579(d). In fixed-width
 *            builds the transform is done in 256-bit integers, so it stays
 *            exact for every term that fits
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 33, u128 n <= 60,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 968 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static int A7_seq_is_big(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x = x * 2;
    return !(x == 0);
}

static seq_t A7_u128_to_seq(unsigned __int128 u)
{
    seq_t r = 0;
    int k;
    for (k = 3; k >= 0; k--) {
        r = r * 65536ULL;
        r = r * 65536ULL;
        r += (seq_t)(uint64_t)(uint32_t)(u >> (32 * k));
    }
    return r;
}

typedef struct { uint32_t *d; size_t len, cap; } A7_big_t;

static void A7_reserve(A7_big_t *b, size_t c) {
    if (c > b->cap) {
        size_t nc = c * 2 + 8;
        b->d = (uint32_t *)realloc(b->d, nc * sizeof(uint32_t));
        if (b->d == NULL) abort();
        b->cap = nc;
    }
}

static void A7_trim(A7_big_t *b) {
    while (b->len && b->d[b->len - 1] == 0) b->len--;
}

static void A7_set_small(A7_big_t *b, uint32_t v) {
    A7_reserve(b, 1);
    b->d[0] = v;
    b->len = 1;
    A7_trim(b);
}

static void A7_mul(A7_big_t *dst, const A7_big_t *src, uint64_t m) {
    size_t k;
    unsigned __int128 carry = 0;
    A7_reserve(dst, src->len + 4);
    for (k = 0; k < src->len; k++) {
        unsigned __int128 cur = (unsigned __int128)src->d[k] * m + carry;
        dst->d[k] = (uint32_t)cur;
        carry = cur >> 32;
    }
    while (carry) {
        dst->d[k++] = (uint32_t)carry;
        carry >>= 32;
    }
    dst->len = k;
    A7_trim(dst);
}

static void A7_add(A7_big_t *dst, const A7_big_t *src) {
    size_t mx = dst->len > src->len ? dst->len : src->len;
    size_t k;
    uint64_t carry = 0;
    A7_reserve(dst, mx + 1);
    for (k = 0; k < mx; k++) {
        uint64_t s = carry;
        if (k < dst->len) s += dst->d[k];
        if (k < src->len) s += src->d[k];
        dst->d[k] = (uint32_t)s;
        carry = s >> 32;
    }
    if (carry) dst->d[k++] = (uint32_t)carry;
    dst->len = k;
}

static void A7_sub(A7_big_t *dst, const A7_big_t *src) {
    size_t k;
    int64_t borrow = 0;
    for (k = 0; k < dst->len; k++) {
        int64_t s = (int64_t)dst->d[k] - borrow - (int64_t)(k < src->len ? src->d[k] : 0);
        if (s < 0) { s += (int64_t)1 << 32; borrow = 1; } else borrow = 0;
        dst->d[k] = (uint32_t)s;
    }
    A7_trim(dst);
}

static void A7_div(A7_big_t *b, uint32_t d) {
    uint64_t rem = 0;
    size_t k = b->len;
    while (k > 0) {
        k--;
        rem = (rem << 32) | b->d[k];
        b->d[k] = (uint32_t)(rem / d);
        rem %= d;
    }
    A7_trim(b);
}

static seq_t A7_to_seq(const A7_big_t *b) {
    size_t k;
    if (A7_seq_is_big()) {
        seq_t v = 0;
        seq_t base = (seq_t)65536;
        k = b->len;
        while (k > 0) {
            k--;
            v = v * base * base + (seq_t)(uint64_t)b->d[k];
        }
        return v;
    } else {
        unsigned __int128 v = 0;
        k = b->len < 4 ? b->len : 4;
        while (k > 0) {
            k--;
            v = (v << 32) | b->d[k];
        }
        return A7_u128_to_seq(v);
    }
}

static A7_big_t *A7_M = NULL;
static idx_t A7_mcap = 0;
static idx_t A7_done = 0;

/* ensures A7_M[0..n] are computed */
static void A007579_fill(idx_t n)
{
    if (A7_M == NULL) {
        A7_mcap = 16;
        A7_M = (A7_big_t *)calloc(A7_mcap, sizeof(A7_big_t));
        if (A7_M == NULL) abort();
        A7_set_small(&A7_M[0], 1);
        A7_set_small(&A7_M[1], 1);
        A7_set_small(&A7_M[2], 2);
        A7_set_small(&A7_M[3], 4);
        A7_done = 4;
    }
    if (n >= A7_mcap) {
        idx_t nc = A7_mcap, j;
        A7_big_t *nm;
        while (nc <= n) nc *= 2;
        nm = (A7_big_t *)realloc(A7_M, nc * sizeof(A7_big_t));
        if (nm == NULL) abort();
        for (j = A7_mcap; j < nc; j++) { nm[j].d = NULL; nm[j].len = 0; nm[j].cap = 0; }
        A7_M = nm;
        A7_mcap = nc;
    }
    if (n >= A7_done) {
        idx_t i;
        A7_big_t T2;
        T2.d = NULL; T2.len = 0; T2.cap = 0;
        for (i = A7_done; i <= n; i++) {
            uint64_t c1 = 4 * (5 * i * i + 46 * i + 84);
            uint64_t c2 = 4 * (i - 1) * (10 * i * i + 58 * i + 33);
            uint64_t c3 = 144 * (i - 1) * (i - 2);
            uint64_t c4 = c3 * (i - 3);
            A7_big_t *T1 = &A7_M[i];

            A7_mul(T1, &A7_M[i - 1], c1);
            A7_mul(&T2, &A7_M[i - 2], c2);
            A7_add(T1, &T2);
            A7_mul(&T2, &A7_M[i - 3], c3);
            A7_sub(T1, &T2);
            A7_mul(&T2, &A7_M[i - 4], c4);
            A7_sub(T1, &T2);
            A7_div(T1, (uint32_t)(i + 5));
            A7_div(T1, (uint32_t)(i + 8));
            A7_div(T1, (uint32_t)(i + 9));
        }
        free(T2.d);
        A7_done = n + 1;
    }
}

static seq_t A007579_a(idx_t n)
{
    A007579_fill(n);
    return A7_to_seq(&A7_M[n]);
}

/* ---------- 256-bit fixed arithmetic for fixed-width builds ---------- */
#define W8N 8
typedef struct { uint32_t w[W8N]; } w8_t;

static void w8_zero(w8_t *x) { int i; for (i = 0; i < W8N; i++) x->w[i] = 0; }

static void w8_from_big(w8_t *x, const A7_big_t *b)
{
    size_t i;
    w8_zero(x);
    for (i = 0; i < b->len && i < W8N; i++) x->w[i] = b->d[i];
}

static void w8_add(w8_t *x, const w8_t *y)
{
    uint64_t carry = 0;
    int i;
    for (i = 0; i < W8N; i++) {
        uint64_t s = (uint64_t)x->w[i] + y->w[i] + carry;
        x->w[i] = (uint32_t)s;
        carry = s >> 32;
    }
}

/* r = x * y truncated to 256 bits */
static void w8_mul(w8_t *r, const w8_t *x, const w8_t *y)
{
    int i, j;
    w8_zero(r);
    for (i = 0; i < W8N; i++) {
        uint64_t carry = 0;
        if (x->w[i] == 0) continue;
        for (j = 0; i + j < W8N; j++) {
            uint64_t cur = (uint64_t)x->w[i] * y->w[j] + r->w[i + j] + carry;
            r->w[i + j] = (uint32_t)cur;
            carry = cur >> 32;
        }
    }
}

static void w8_mul_small(w8_t *x, uint32_t m)
{
    uint64_t carry = 0;
    int i;
    for (i = 0; i < W8N; i++) {
        uint64_t cur = (uint64_t)x->w[i] * m + carry;
        x->w[i] = (uint32_t)cur;
        carry = cur >> 32;
    }
}

static void w8_div_small(w8_t *x, uint32_t d)
{
    uint64_t rem = 0;
    int i;
    for (i = W8N - 1; i >= 0; i--) {
        rem = (rem << 32) | x->w[i];
        x->w[i] = (uint32_t)(rem / d);
        rem %= d;
    }
}

static seq_t w8_to_seq(const w8_t *x)
{
    unsigned __int128 v = 0;
    int i;
    for (i = 3; i >= 0; i--) v = (v << 32) | x->w[i];
    return A7_u128_to_seq(v);
}

#define FIX_MAXN 80   /* beyond this no term fits in 128 bits */

static w8_t *f_res = NULL;
static idx_t f_have = 0;

static seq_t a_fixed(idx_t n)
{
    if (n > FIX_MAXN) return 0;   /* value cannot fit in any fixed-width seq_t */
    if (n >= f_have) {
        idx_t N = FIX_MAXN, d, k, m;
        w8_t *c = (w8_t *)calloc(N + 1, sizeof(w8_t));
        w8_t *r = (w8_t *)calloc(N + 1, sizeof(w8_t));
        w8_t t, p;
        if (c == NULL || r == NULL) abort();
        A007579_fill(N);
        for (d = 1; d <= N; d++) {
            w8_from_big(&t, &A7_M[d]);
            w8_mul_small(&t, (uint32_t)d);
            for (k = d; k <= N; k += d) w8_add(&c[k], &t);
        }
        w8_zero(&r[0]);
        r[0].w[0] = 1;
        for (m = 1; m <= N; m++) {
            w8_t s;
            w8_zero(&s);
            for (k = 1; k <= m; k++) {
                w8_mul(&p, &c[k], &r[m - k]);
                w8_add(&s, &p);
            }
            w8_div_small(&s, (uint32_t)m);
            r[m] = s;
        }
        free(c);
        if (f_res != NULL) free(f_res);
        f_res = r;
        f_have = N + 1;
    }
    return w8_to_seq(&f_res[n]);
}

/* ---------- arbitrary-precision path ---------- */
static seq_t *g_res = NULL;
static idx_t g_have = 0;

static seq_t a_big(idx_t n)
{
    if (n >= g_have) {
        idx_t N = n;
        idx_t d, k, m;
        seq_t *c, *r;
        if (N < 2 * g_have) N = 2 * g_have;
        if (N < 16) N = 16;
        c = mem_new(seq_t, N + 1);
        r = mem_new(seq_t, N + 1);
        for (d = 1; d <= N; d++) {
            seq_t t = (seq_t)d * A007579_a(d);
            for (k = d; k <= N; k += d) c[k] += t;
        }
        r[0] = 1;
        for (m = 1; m <= N; m++) {
            seq_t s = 0;
            for (k = 1; k <= m; k++) s += c[k] * r[m - k];
            r[m] = s / (seq_t)m;
        }
        mem_free(c);
        if (g_res != NULL) mem_free(g_res);
        g_res = r;
        g_have = N + 1;
    }
    return g_res[n];
}

seq_t a(idx_t n)
{
    if (A7_seq_is_big()) return a_big(n);
    return a_fixed(n);
}
