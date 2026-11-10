/*
 * A374133  The 3-adic valuation of A276085(n), where A276085 is the primorial
 *          base log-function.
 * https://oeis.org/A374133
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007949(A276085(n)), where A276085(n) = Sum_{p^e || n} e
 *            * (product of primes < p). The sum is computed exactly in base
 *            3^19 limbs, so no intermediate value overflows and the 3-adic
 *            valuation can be read off directly
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 124 terms through n = 9650 (2026-11-10)
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

#define A374133_B 1162261467ULL /* 3^19 */

typedef struct {
    uint64_t *d;
    size_t len;
    size_t cap;
} A374133_num;

static void A374133_reserve(A374133_num *x, size_t need)
{
    if (need <= x->cap) return;
    size_t nc = x->cap ? x->cap : 16;
    while (nc < need) nc *= 2;
    uint64_t *nd = (uint64_t *)realloc(x->d, nc * sizeof(uint64_t));
    if (!nd) abort();
    for (size_t i = x->cap; i < nc; ++i) nd[i] = 0;
    x->d = nd;
    x->cap = nc;
}

/* x *= m, with m < 2^32 */
static void A374133_mul_small(A374133_num *x, uint64_t m)
{
    uint64_t carry = 0;
    for (size_t i = 0; i < x->len; ++i) {
        uint64_t t = x->d[i] * m + carry;
        x->d[i] = t % A374133_B;
        carry = t / A374133_B;
    }
    while (carry) {
        A374133_reserve(x, x->len + 1);
        x->d[x->len++] = carry % A374133_B;
        carry /= A374133_B;
    }
}

/* s += p * e, with e small */
static void A374133_add_scaled(A374133_num *s, const A374133_num *p, uint64_t e)
{
    size_t L = p->len > s->len ? p->len : s->len;
    A374133_reserve(s, L + 1);
    for (size_t i = s->len; i < L + 1; ++i) s->d[i] = 0;
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < L; ++i) {
        uint64_t pv = i < p->len ? p->d[i] : 0;
        uint64_t t = s->d[i] + pv * e + carry;
        s->d[i] = t % A374133_B;
        carry = t / A374133_B;
    }
    s->len = L;
    while (carry) {
        A374133_reserve(s, s->len + 1);
        s->d[s->len++] = carry % A374133_B;
        carry /= A374133_B;
    }
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;
    uint64_t facs[64];
    uint64_t exps[64];
    size_t fcnt = 0;
    factor_u64((uint64_t)n, facs, exps, &fcnt);

    /* sort factors ascending */
    for (size_t i = 1; i < fcnt; ++i) {
        uint64_t f = facs[i], e = exps[i];
        size_t j = i;
        while (j > 0 && facs[j - 1] > f) {
            facs[j] = facs[j - 1];
            exps[j] = exps[j - 1];
            --j;
        }
        facs[j] = f;
        exps[j] = e;
    }
    uint64_t max_p = fcnt ? facs[fcnt - 1] : 0;
    if (max_p == 0) return (seq_t)0;

    uint64_t *pl = NULL;
    size_t pc = 0;
    build_primes_upto((idx_t)max_p, &pl, &pc);

    A374133_num P = {NULL, 0, 0};
    A374133_num S = {NULL, 0, 0};
    A374133_reserve(&P, 1);
    P.d[0] = 1;
    P.len = 1;

    size_t fi = 0;
    size_t pj = 0;
    while (fi < fcnt) {
        /* advance primorial P to product of primes < facs[fi] */
        while (pj < pc && pl[pj] < facs[fi]) {
            A374133_mul_small(&P, pl[pj]);
            ++pj;
        }
        A374133_add_scaled(&S, &P, exps[fi]);
        ++fi;
    }
    free(pl);

    unsigned long long count = 0;
    size_t i = 0;
    while (i < S.len && S.d[i] == 0) {
        count += 19;
        ++i;
    }
    if (i == S.len) {
        free(P.d);
        free(S.d);
        return (seq_t)0;
    }
    uint64_t v = S.d[i];
    while (v % 3 == 0) {
        ++count;
        v /= 3;
    }
    free(P.d);
    free(S.d);
    return (seq_t)count;
}
