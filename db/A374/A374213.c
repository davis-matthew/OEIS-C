/*
 * A374213  The 3-adic valuation of A328768(n), where A328768 is the first
 *          primorial based variant of the arithmetic derivative.
 * https://oeis.org/A374213
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007949(A328768(n)), where A328768(n) = Sum_{p^e || n}
 *            e*(n/p)*A002110(primepi(p)-1); A328768(n) is computed exactly
 *            with a small internal bignum, then its 3-adic valuation is taken
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 123 terms through n = 9681 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct { uint32_t *d; size_t len, cap; } A374213_big;

static void A374213_reserve(A374213_big *b, size_t need)
{
    if (need <= b->cap) return;
    size_t nc = b->cap ? b->cap : 16;
    while (nc < need) nc *= 2;
    b->d = (uint32_t *)realloc(b->d, nc * sizeof(uint32_t));
    memset(b->d + b->cap, 0, (nc - b->cap) * sizeof(uint32_t));
    b->cap = nc;
}

static void A374213_set_u32(A374213_big *b, uint32_t v)
{
    A374213_reserve(b, 1);
    b->d[0] = v;
    b->len = v ? 1 : 0;
}

static void A374213_copy(A374213_big *dst, const A374213_big *src)
{
    A374213_reserve(dst, src->len + 1);
    if (src->len) memcpy(dst->d, src->d, src->len * sizeof(uint32_t));
    dst->len = src->len;
}

static void A374213_mul_small(A374213_big *b, uint32_t m)
{
    if (b->len == 0) return;
    if (m == 0) { b->len = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < b->len; ++i) {
        uint64_t t = (uint64_t)b->d[i] * m + carry;
        b->d[i] = (uint32_t)t;
        carry = t >> 32;
    }
    if (carry) {
        A374213_reserve(b, b->len + 1);
        b->d[b->len++] = (uint32_t)carry;
    }
}

static void A374213_add(A374213_big *s, const A374213_big *t)
{
    size_t n = s->len > t->len ? s->len : t->len;
    A374213_reserve(s, n + 1);
    uint64_t carry = 0;
    for (size_t i = 0; i < n; ++i) {
        uint64_t x = (i < s->len ? s->d[i] : 0);
        uint64_t y = (i < t->len ? t->d[i] : 0);
        uint64_t r = x + y + carry;
        s->d[i] = (uint32_t)r;
        carry = r >> 32;
    }
    s->len = n;
    if (carry) s->d[s->len++] = (uint32_t)carry;
}

/* returns remainder, divides in place */
static uint32_t A374213_divmod_small(A374213_big *b, uint32_t m)
{
    uint64_t rem = 0;
    for (size_t i = b->len; i-- > 0;) {
        uint64_t cur = (rem << 32) | b->d[i];
        b->d[i] = (uint32_t)(cur / m);
        rem = cur % m;
    }
    while (b->len && b->d[b->len - 1] == 0) b->len--;
    return (uint32_t)rem;
}

static uint32_t A374213_mod_small(const A374213_big *b, uint32_t m)
{
    uint64_t rem = 0;
    for (size_t i = b->len; i-- > 0;)
        rem = ((rem << 32) | b->d[i]) % m;
    return (uint32_t)rem;
}

static bool A374213_isprime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    uint64_t N = (uint64_t)n;

    /* factor n */
    uint64_t pr[64];
    unsigned ex[64];
    int cnt = 0;
    {
        uint64_t m = N;
        for (uint64_t d = 2; d * d <= m; ++d) {
            if (m % d == 0) {
                unsigned e = 0;
                while (m % d == 0) { m /= d; ++e; }
                pr[cnt] = d; ex[cnt] = e; ++cnt;
            }
        }
        if (m > 1) { pr[cnt] = m; ex[cnt] = 1; ++cnt; }
    }
    uint64_t maxp = pr[cnt - 1];

    A374213_big P = {NULL, 0, 0}, S = {NULL, 0, 0}, T = {NULL, 0, 0};
    A374213_set_u32(&P, 1);
    A374213_set_u32(&S, 0);

    int k = 0;
    for (uint64_t q = 2; q <= maxp; ++q) {
        if (!A374213_isprime(q)) continue;
        if (k < cnt && pr[k] == q) {
            /* T = P * e * (n/q) */
            A374213_copy(&T, &P);
            A374213_mul_small(&T, ex[k]);
            for (int j = 0; j < cnt; ++j) {
                unsigned f = ex[j] - (j == k ? 1u : 0u);
                for (unsigned t = 0; t < f; ++t)
                    A374213_mul_small(&T, (uint32_t)pr[j]);
            }
            A374213_add(&S, &T);
            ++k;
        }
        if (q < maxp) A374213_mul_small(&P, (uint32_t)q);
    }

    unsigned count = 0;
    if (S.len) {
        while (A374213_mod_small(&S, 3) == 0) {
            A374213_divmod_small(&S, 3);
            ++count;
        }
    }
    free(P.d); free(S.d); free(T.d);
    return (seq_t)count;
}
