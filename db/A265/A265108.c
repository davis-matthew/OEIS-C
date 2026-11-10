/*
 * A265108  The number of n-digit numbers in A264847 (pluritriangular
 *          numbers).
 * https://oeis.org/A265108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of terms of A264847 with exactly n digits (a(0) =
 *            0). Inside one decade the terms form a quadratic progression x_k
 *            = x_0 + k*S_0 + n*k*(k-1)/2, where S_0 is the first gap. For
 *            each decade, binary search finds the last term below 10^n.
 *            Fixed-width multi-limb integers hold the interme...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 78,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 90 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define A265108_NL 48

typedef struct { uint32_t v[A265108_NL]; } A265108_big;

static int A265108_len(const A265108_big *a)
{
    int l = A265108_NL;
    while (l > 0 && a->v[l - 1] == 0) --l;
    return l;
}

static void A265108_set(A265108_big *r, uint64_t x)
{
    memset(r->v, 0, sizeof(r->v));
    r->v[0] = (uint32_t)(x & 0xffffffffu);
    r->v[1] = (uint32_t)(x >> 32);
}

static void A265108_add(A265108_big *r, const A265108_big *a, const A265108_big *b)
{
    uint64_t c = 0;
    int i;
    for (i = 0; i < A265108_NL; ++i) {
        uint64_t t = (uint64_t)a->v[i] + b->v[i] + c;
        r->v[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void A265108_sub(A265108_big *r, const A265108_big *a, const A265108_big *b)
{
    int64_t br = 0;
    int i;
    for (i = 0; i < A265108_NL; ++i) {
        int64_t t = (int64_t)a->v[i] - (int64_t)b->v[i] - br;
        if (t < 0) { t += ((int64_t)1 << 32); br = 1; } else br = 0;
        r->v[i] = (uint32_t)t;
    }
}

static void A265108_mul(A265108_big *r, const A265108_big *a, const A265108_big *b)
{
    A265108_big t;
    int la = A265108_len(a), lb = A265108_len(b), i, j;
    memset(t.v, 0, sizeof(t.v));
    for (i = 0; i < la; ++i) {
        uint64_t c = 0;
        if (a->v[i] == 0) continue;
        for (j = 0; j < lb && i + j < A265108_NL; ++j) {
            uint64_t p = (uint64_t)a->v[i] * b->v[j] + t.v[i + j] + c;
            t.v[i + j] = (uint32_t)p;
            c = p >> 32;
        }
        for (j = i + lb; c && j < A265108_NL; ++j) {
            uint64_t p = (uint64_t)t.v[j] + c;
            t.v[j] = (uint32_t)p;
            c = p >> 32;
        }
    }
    *r = t;
}

static void A265108_mulsmall(A265108_big *r, const A265108_big *a, uint32_t m)
{
    uint64_t c = 0;
    int i;
    for (i = 0; i < A265108_NL; ++i) {
        uint64_t p = (uint64_t)a->v[i] * m + c;
        r->v[i] = (uint32_t)p;
        c = p >> 32;
    }
}

static void A265108_shr1(A265108_big *r)
{
    int i;
    for (i = 0; i < A265108_NL; ++i) {
        uint32_t hi = (i + 1 < A265108_NL) ? (r->v[i + 1] & 1u) : 0u;
        r->v[i] = (r->v[i] >> 1) | (hi << 31);
    }
}

static int A265108_cmp(const A265108_big *a, const A265108_big *b)
{
    int i;
    for (i = A265108_NL - 1; i >= 0; --i) {
        if (a->v[i] != b->v[i]) return a->v[i] < b->v[i] ? -1 : 1;
    }
    return 0;
}

static bool A265108_iszero(const A265108_big *a)
{
    return A265108_len(a) == 0;
}

/* r = x + k*S + d*k*(k-1)/2 */
static void A265108_f(A265108_big *r, const A265108_big *x, const A265108_big *S,
                      uint32_t d, const A265108_big *k)
{
    A265108_big t, one, km1, u;
    A265108_set(&t, 0);
    if (!A265108_iszero(k)) {
        A265108_set(&one, 1);
        A265108_sub(&km1, k, &one);
        A265108_mul(&t, k, &km1);
        A265108_shr1(&t);
        A265108_mulsmall(&t, &t, d);
    }
    A265108_mul(&u, k, S);
    A265108_add(&t, &t, &u);
    A265108_add(r, &t, x);
}

static seq_t A265108_toseq(const A265108_big *a)
{
    seq_t res = 0;
    int i = A265108_len(a) - 1;
    for (; i >= 0; --i) {
        res = res * 65536 + (seq_t)(a->v[i] >> 16);
        res = res * 65536 + (seq_t)(a->v[i] & 0xffffu);
    }
    return res;
}

seq_t a(idx_t n)
{
    A265108_big x, S, limit, hi, lo, mid, fv, one, diff, Sk, xk, lim2;
    idx_t d;
    if (n == 0) return 0;
    A265108_set(&x, 0);
    A265108_set(&S, 1);
    A265108_set(&limit, 1);
    A265108_set(&one, 1);
    for (d = 1; d <= n; ++d) {
        A265108_mulsmall(&limit, &limit, 10);
        if (A265108_cmp(&x, &limit) >= 0) {
            if (d == n) return 0;
            continue;
        }
        /* largest k with f(k) < limit */
        A265108_set(&hi, 1);
        for (;;) {
            A265108_f(&fv, &x, &S, (uint32_t)d, &hi);
            if (A265108_cmp(&fv, &limit) >= 0) break;
            A265108_add(&hi, &hi, &hi);
        }
        A265108_set(&lo, 0);
        for (;;) {
            A265108_sub(&diff, &hi, &lo);
            if (A265108_cmp(&diff, &one) <= 0) break;
            A265108_add(&mid, &lo, &hi);
            A265108_shr1(&mid);
            A265108_f(&fv, &x, &S, (uint32_t)d, &mid);
            if (A265108_cmp(&fv, &limit) < 0) lo = mid; else hi = mid;
        }
        if (d == n) {
            A265108_add(&lo, &lo, &one);
            return A265108_toseq(&lo);
        }
        A265108_f(&xk, &x, &S, (uint32_t)d, &lo);
        A265108_mulsmall(&Sk, &lo, (uint32_t)d);
        A265108_add(&Sk, &Sk, &S);
        A265108_add(&x, &xk, &Sk);
        {
            idx_t dd = d;
            lim2 = limit;
            while (A265108_cmp(&x, &lim2) >= 0) {
                A265108_mulsmall(&lim2, &lim2, 10);
                ++dd;
            }
            A265108_set(&fv, (uint64_t)dd);
            A265108_add(&S, &Sk, &fv);
        }
    }
    return 0;
}
