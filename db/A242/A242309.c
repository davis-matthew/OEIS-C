/*
 * A242309  a(n) = A211384(2n)/A211384(n).
 * https://oeis.org/A242309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A211384(2n)/A211384(n), where A211384(1) = 1, A211384(2)
 *            = 3 and for m > 2, A211384(m) = (floor(A211384(m-1)/L) + 1) * L
 *            with L = lcm{ A211384(m/p) : p prime, p | m }. A211384 grows
 *            past 64 bits, so it is computed exactly with an internal
 *            multiprecision integer; only the small quotient is...
 * Range:     u8 n <= 296, u16 n <= 1385, u32 n <= 7559, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9231 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uint32_t *d; size_t n, cap; } A242309_bn;

static void bn_reserve(A242309_bn *a, size_t c)
{
    if (c <= a->cap) return;
    size_t nc = a->cap ? a->cap * 2 : 8;
    while (nc < c) nc *= 2;
    uint32_t *p = (uint32_t *)realloc(a->d, nc * sizeof(uint32_t));
    if (!p) abort();
    a->d = p;
    a->cap = nc;
}

static void bn_norm(A242309_bn *a)
{
    while (a->n && a->d[a->n - 1] == 0) a->n--;
}

static void bn_set_u32(A242309_bn *a, uint32_t v)
{
    bn_reserve(a, 1);
    a->d[0] = v;
    a->n = v ? 1 : 0;
}

static void bn_copy(A242309_bn *r, const A242309_bn *a)
{
    bn_reserve(r, a->n + 1);
    if (a->n) memcpy(r->d, a->d, a->n * sizeof(uint32_t));
    r->n = a->n;
}

static int bn_cmp(const A242309_bn *a, const A242309_bn *b)
{
    if (a->n != b->n) return a->n < b->n ? -1 : 1;
    for (size_t k = a->n; k-- > 0;) {
        if (a->d[k] != b->d[k]) return a->d[k] < b->d[k] ? -1 : 1;
    }
    return 0;
}

/* a -= b, requires a >= b */
static void bn_sub(A242309_bn *a, const A242309_bn *b)
{
    uint64_t borrow = 0;
    for (size_t k = 0; k < a->n; ++k) {
        uint64_t sub = borrow + (k < b->n ? (uint64_t)b->d[k] : 0);
        uint64_t cur = (uint64_t)a->d[k];
        if (cur >= sub) { a->d[k] = (uint32_t)(cur - sub); borrow = 0; }
        else { a->d[k] = (uint32_t)(cur + ((uint64_t)1 << 32) - sub); borrow = 1; }
        if (!borrow && k >= b->n) break;
    }
    bn_norm(a);
}

static size_t bn_bitlen(const A242309_bn *a)
{
    if (a->n == 0) return 0;
    uint32_t top = a->d[a->n - 1];
    size_t b = 0;
    while (top) { b++; top >>= 1; }
    return (a->n - 1) * 32 + b;
}

/* r = a << s (r must not alias a) */
static void bn_shl(A242309_bn *r, const A242309_bn *a, size_t s)
{
    size_t limbs = s / 32, bits = s % 32;
    size_t len = a->n + limbs + 1;
    bn_reserve(r, len);
    memset(r->d, 0, len * sizeof(uint32_t));
    for (size_t k = 0; k < a->n; ++k) {
        uint64_t v = (uint64_t)a->d[k] << bits;
        r->d[k + limbs] |= (uint32_t)v;
        r->d[k + limbs + 1] |= (uint32_t)(v >> 32);
    }
    r->n = len;
    bn_norm(r);
}

static void bn_shr1(A242309_bn *a)
{
    for (size_t k = 0; k < a->n; ++k) {
        uint32_t hi = (k + 1 < a->n) ? (a->d[k + 1] << 31) : 0;
        a->d[k] = (a->d[k] >> 1) | hi;
    }
    bn_norm(a);
}

/* r = a * b (r must not alias a or b) */
static void bn_mul(A242309_bn *r, const A242309_bn *a, const A242309_bn *b)
{
    if (a->n == 0 || b->n == 0) { r->n = 0; return; }
    size_t len = a->n + b->n;
    bn_reserve(r, len);
    memset(r->d, 0, len * sizeof(uint32_t));
    for (size_t i = 0; i < a->n; ++i) {
        uint64_t carry = 0;
        for (size_t j = 0; j < b->n; ++j) {
            uint64_t cur = (uint64_t)a->d[i] * b->d[j] + r->d[i + j] + carry;
            r->d[i + j] = (uint32_t)cur;
            carry = cur >> 32;
        }
        size_t k = i + b->n;
        while (carry) {
            uint64_t cur = (uint64_t)r->d[k] + carry;
            r->d[k] = (uint32_t)cur;
            carry = cur >> 32;
            k++;
        }
    }
    r->n = len;
    bn_norm(r);
}

static void bn_add_u32(A242309_bn *a, uint32_t v)
{
    bn_reserve(a, a->n + 1);
    uint64_t carry = v;
    size_t k = 0;
    while (carry) {
        if (k == a->n) { a->d[k] = 0; a->n++; }
        uint64_t cur = (uint64_t)a->d[k] + carry;
        a->d[k] = (uint32_t)cur;
        carry = cur >> 32;
        k++;
    }
}

static A242309_bn bn_div_t = { NULL, 0, 0 };

/* q = a / b, r = a % b ; q, r distinct from a, b and each other; b != 0 */
static void bn_divmod(A242309_bn *q, A242309_bn *r, const A242309_bn *a, const A242309_bn *b)
{
    bn_copy(r, a);
    if (bn_cmp(a, b) < 0) { q->n = 0; return; }
    size_t k = bn_bitlen(a) - bn_bitlen(b);
    bn_shl(&bn_div_t, b, k);
    size_t ql = k / 32 + 1;
    bn_reserve(q, ql);
    memset(q->d, 0, ql * sizeof(uint32_t));
    q->n = ql;
    for (size_t s = k + 1; s-- > 0;) {
        if (bn_cmp(r, &bn_div_t) >= 0) {
            bn_sub(r, &bn_div_t);
            q->d[s / 32] |= (uint32_t)1 << (s % 32);
        }
        bn_shr1(&bn_div_t);
    }
    bn_norm(q);
}

static A242309_bn bn_g_x = { NULL, 0, 0 }, bn_g_y = { NULL, 0, 0 },
                  bn_g_r = { NULL, 0, 0 }, bn_g_q = { NULL, 0, 0 };

/* g = gcd(a, b) */
static void bn_gcd(A242309_bn *g, const A242309_bn *a, const A242309_bn *b)
{
    bn_copy(&bn_g_x, a);
    bn_copy(&bn_g_y, b);
    A242309_bn *X = &bn_g_x, *Y = &bn_g_y, *R = &bn_g_r;
    while (Y->n) {
        bn_divmod(&bn_g_q, R, X, Y);
        A242309_bn *t = X; X = Y; Y = R; R = t;
    }
    bn_copy(g, X);
}

static A242309_bn *A211384_cache = NULL;
static size_t A211384_cap = 0;
static size_t A211384_done = 0;

static A242309_bn s_L = { NULL, 0, 0 }, s_G = { NULL, 0, 0 }, s_Q = { NULL, 0, 0 },
                  s_R = { NULL, 0, 0 }, s_T = { NULL, 0, 0 };

static void A211384_compute(size_t n)
{
    if (n <= A211384_done) return;
    if (n + 1 > A211384_cap) {
        size_t nc = A211384_cap ? A211384_cap * 2 : 64;
        while (nc < n + 1) nc *= 2;
        A242309_bn *p = (A242309_bn *)realloc(A211384_cache, nc * sizeof(A242309_bn));
        if (!p) abort();
        memset(p + A211384_cap, 0, (nc - A211384_cap) * sizeof(A242309_bn));
        A211384_cache = p;
        A211384_cap = nc;
    }
    for (size_t i = A211384_done + 1; i <= n; ++i) {
        A242309_bn *ci = &A211384_cache[i];
        if (i == 1) { bn_set_u32(ci, 1); continue; }
        if (i == 2) { bn_set_u32(ci, 3); continue; }

        size_t primes[32];
        int np = 0;
        size_t m = i;
        for (size_t p = 2; p * p <= m; ++p) {
            if (m % p == 0) {
                primes[np++] = p;
                while (m % p == 0) m /= p;
            }
        }
        if (m > 1) primes[np++] = m;

        bn_copy(&s_L, &A211384_cache[i / primes[0]]);
        for (int k = 1; k < np; ++k) {
            const A242309_bn *x = &A211384_cache[i / primes[k]];
            bn_gcd(&s_G, &s_L, x);
            bn_divmod(&s_Q, &s_R, &s_L, &s_G);
            bn_mul(&s_T, &s_Q, x);
            bn_copy(&s_L, &s_T);
        }

        const A242309_bn *prev = &A211384_cache[i - 1];
        if (s_L.n == 1 && s_L.d[0] == 1) {
            bn_copy(&s_Q, prev);
        } else {
            bn_divmod(&s_Q, &s_R, prev, &s_L);
        }
        bn_add_u32(&s_Q, 1);
        bn_mul(&s_T, &s_Q, &s_L);
        bn_copy(ci, &s_T);
    }
    A211384_done = n;
}

static A242309_bn s_fq = { NULL, 0, 0 }, s_fr = { NULL, 0, 0 };

seq_t a(idx_t n)
{
    size_t m = (size_t)n;
    if (m == 0) return (seq_t)0;
    A211384_compute(2 * m);
    bn_divmod(&s_fq, &s_fr, &A211384_cache[2 * m], &A211384_cache[m]);
    seq_t v = (seq_t)0;
    for (size_t k = s_fq.n; k-- > 0;) {
        uint32_t limb = s_fq.d[k];
        v = v * (seq_t)256;
        v = v * (seq_t)256;
        v = v + (seq_t)(limb >> 16);
        v = v * (seq_t)256;
        v = v * (seq_t)256;
        v = v + (seq_t)(limb & 0xFFFFu);
    }
    return v;
}
