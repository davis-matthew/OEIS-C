/*
 * A374034  a(n) = A276150(gcd(A276085(n), A328768(n))), where A276150 is the
 *          digit sum in primorial base, A276085 is the primorial base
 *          log-function, and A328768 is the first primorial based variant of
 *          arithmetic derivative.
 * https://oeis.org/A374034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A276150(gcd(A276085(n), A328768(n))), where A276085(n) =
 *            Sum_{p^e || n} e*(p#/p) and A328768(n) = Sum_{p^e || n}
 *            e*(p#/p)*(n/p); computed with an internal multiprecision integer
 *            so that huge intermediates never overflow
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 121 terms through n = 9958 (2026-11-10)
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

typedef struct { uint32_t *d; size_t n; size_t cap; } A374034_bn;

static void A374034_reserve(A374034_bn *x, size_t cap)
{
    if (cap > x->cap) {
        x->d = (uint32_t *)realloc(x->d, cap * sizeof(uint32_t));
        memset(x->d + x->cap, 0, (cap - x->cap) * sizeof(uint32_t));
        x->cap = cap;
    }
}

static void A374034_trim(A374034_bn *x)
{
    while (x->n > 0 && x->d[x->n - 1] == 0) x->n--;
}

static void A374034_set_u32(A374034_bn *x, uint32_t v)
{
    A374034_reserve(x, 4);
    memset(x->d, 0, x->cap * sizeof(uint32_t));
    x->d[0] = v;
    x->n = 1;
    A374034_trim(x);
}

static void A374034_copy(A374034_bn *dst, const A374034_bn *src)
{
    A374034_reserve(dst, src->n + 4);
    memset(dst->d, 0, dst->cap * sizeof(uint32_t));
    if (src->n) memcpy(dst->d, src->d, src->n * sizeof(uint32_t));
    dst->n = src->n;
}

/* dst += src * m * 2^(32*shift) */
static void A374034_addmul_u32(A374034_bn *dst, const A374034_bn *src, uint32_t m, size_t shift)
{
    if (m == 0 || src->n == 0) return;
    size_t need = (dst->n > src->n + shift ? dst->n : src->n + shift) + 2;
    A374034_reserve(dst, need);
    for (size_t i = dst->n; i < need; ++i) dst->d[i] = 0;
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < src->n; ++i) {
        size_t idx = i + shift;
        uint64_t t = (uint64_t)src->d[i] * m + dst->d[idx] + carry;
        dst->d[idx] = (uint32_t)t;
        carry = t >> 32;
    }
    size_t idx = src->n + shift;
    while (carry) {
        uint64_t t = (uint64_t)dst->d[idx] + carry;
        dst->d[idx] = (uint32_t)t;
        carry = t >> 32;
        ++idx;
    }
    dst->n = need;
    A374034_trim(dst);
}

static void A374034_addmul_u64(A374034_bn *dst, const A374034_bn *src, uint64_t m)
{
    A374034_addmul_u32(dst, src, (uint32_t)m, 0);
    A374034_addmul_u32(dst, src, (uint32_t)(m >> 32), 1);
}

static void A374034_mul_u32(A374034_bn *x, uint32_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < x->n; ++i) {
        uint64_t t = (uint64_t)x->d[i] * m + carry;
        x->d[i] = (uint32_t)t;
        carry = t >> 32;
    }
    if (carry) {
        A374034_reserve(x, x->n + 4);
        x->d[x->n++] = (uint32_t)carry;
    }
    A374034_trim(x);
}

static uint32_t A374034_divmod_u32(A374034_bn *x, uint32_t m)
{
    uint64_t r = 0;
    for (size_t i = x->n; i-- > 0;) {
        r = (r << 32) | x->d[i];
        x->d[i] = (uint32_t)(r / m);
        r %= m;
    }
    A374034_trim(x);
    return (uint32_t)r;
}

static int A374034_cmp(const A374034_bn *a, const A374034_bn *b)
{
    if (a->n != b->n) return a->n < b->n ? -1 : 1;
    for (size_t i = a->n; i-- > 0;)
        if (a->d[i] != b->d[i]) return a->d[i] < b->d[i] ? -1 : 1;
    return 0;
}

/* a -= b, requires a >= b */
static void A374034_sub(A374034_bn *a, const A374034_bn *b)
{
    uint64_t borrow = 0;
    for (size_t i = 0; i < a->n; ++i) {
        uint64_t bv = (i < b->n ? b->d[i] : 0) + borrow;
        uint64_t av = a->d[i];
        if (av >= bv) { a->d[i] = (uint32_t)(av - bv); borrow = 0; }
        else { a->d[i] = (uint32_t)(av + ((uint64_t)1 << 32) - bv); borrow = 1; }
    }
    A374034_trim(a);
}

static size_t A374034_ctz(const A374034_bn *x)
{
    size_t i = 0;
    while (i < x->n && x->d[i] == 0) ++i;
    if (i == x->n) return 0;
    uint32_t v = x->d[i];
    size_t c = 0;
    while (!(v & 1u)) { v >>= 1; ++c; }
    return i * 32 + c;
}

static void A374034_shr(A374034_bn *x, size_t k)
{
    size_t limbs = k / 32, bits = k % 32;
    if (limbs >= x->n) { x->n = 0; memset(x->d, 0, x->cap * sizeof(uint32_t)); return; }
    size_t nn = x->n - limbs;
    for (size_t i = 0; i < nn; ++i) {
        uint32_t lo = x->d[i + limbs] >> bits;
        uint32_t hi = (bits && i + limbs + 1 < x->n) ? (x->d[i + limbs + 1] << (32 - bits)) : 0;
        x->d[i] = lo | hi;
    }
    for (size_t i = nn; i < x->n; ++i) x->d[i] = 0;
    x->n = nn;
    A374034_trim(x);
}

static void A374034_shl(A374034_bn *x, size_t k)
{
    if (x->n == 0 || k == 0) return;
    size_t limbs = k / 32, bits = k % 32;
    size_t oldn = x->n;
    size_t nn = oldn + limbs + 1;
    A374034_reserve(x, nn + 1);
    for (long long i = (long long)nn - 1; i >= 0; --i) {
        long long j = i - (long long)limbs;
        uint32_t hi = (j >= 0 && j < (long long)oldn) ? (x->d[j] << bits) : 0;
        uint32_t lo = (bits && j - 1 >= 0 && j - 1 < (long long)oldn) ? (x->d[j - 1] >> (32 - bits)) : 0;
        x->d[i] = hi | lo;
    }
    x->n = nn;
    A374034_trim(x);
}

static bool A374034_isprime(uint64_t x)
{
    if (x < 2) return false;
    if (x < 4) return true;
    if (x % 2 == 0) return false;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    uint64_t facs[64], exps[64];
    size_t fcnt = 0;
    factor_u64((uint64_t)n, facs, exps, &fcnt);

    uint64_t maxp = 0;
    for (size_t i = 0; i < fcnt; ++i)
        if (facs[i] > maxp) maxp = facs[i];

    uint64_t *plist = NULL;
    size_t pcnt = 0;
    build_primes_upto((idx_t)maxp, &plist, &pcnt);

    A374034_bn P = {NULL, 0, 0}, A = {NULL, 0, 0}, B = {NULL, 0, 0}, T = {NULL, 0, 0};
    A374034_set_u32(&P, 1);
    A374034_set_u32(&A, 0);
    A374034_set_u32(&B, 0);
    A374034_set_u32(&T, 0);

    for (size_t k = 0; k < pcnt && plist[k] <= maxp; ++k) {
        uint64_t p = plist[k];
        for (size_t i = 0; i < fcnt; ++i) {
            if (facs[i] == p) {
                A374034_copy(&T, &P);
                A374034_mul_u32(&T, (uint32_t)exps[i]);
                A374034_addmul_u32(&A, &T, 1u, 0);
                A374034_addmul_u64(&B, &T, (uint64_t)n / p);
            }
        }
        if (p < maxp) A374034_mul_u32(&P, (uint32_t)p);
    }
    free(plist);

    /* binary gcd of A and B (both positive for n > 1) */
    size_t sa = A374034_ctz(&A), sb = A374034_ctz(&B);
    size_t sh = sa < sb ? sa : sb;
    A374034_shr(&A, sa);
    while (B.n != 0) {
        A374034_shr(&B, A374034_ctz(&B));
        if (A374034_cmp(&A, &B) > 0) {
            A374034_bn tmp = A; A = B; B = tmp;
        }
        A374034_sub(&B, &A);
    }
    A374034_shl(&A, sh);

    /* digit sum in primorial base */
    uint64_t sum = 0;
    uint64_t p = 2;
    while (A.n != 0) {
        sum += A374034_divmod_u32(&A, (uint32_t)p);
        ++p;
        while (!A374034_isprime(p)) ++p;
    }

    free(P.d); free(A.d); free(B.d); free(T.d);
    return (seq_t)sum;
}
