/*
 * A307227  Numbers k such that A060648(k) is divisible by k.
 * https://oeis.org/A307227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search, not a closed form. A307227 lists the k with k |
 *            A060648(k), where A060648(n) = Prod_{p^e||n}
 *            (p^(e+1)+p^e-2)/(p-1). The code does an exhaustive depth-first
 *            search over factorizations, taking primes in decreasing order. A
 *            branch is pruned when the part of k not yet divided by the
 *            partial A...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 16, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

typedef unsigned __int128 a307_u128;

static uint64_t  A307_B;
static uint64_t *A307_res = NULL;
static size_t    A307_rcnt = 0, A307_rcap = 0;
static uint32_t *A307_pr = NULL;
static size_t    A307_np = 0;
static uint32_t *A307_spf = NULL;
static uint64_t  A307_fstack[8192];
static size_t    A307_ftop = 0;
#define A307_SENT UINT64_MAX

static void A307_rec(uint64_t k)
{
    if (A307_rcnt == A307_rcap) {
        A307_rcap = A307_rcap ? 2 * A307_rcap : 256;
        A307_res = (uint64_t *)realloc(A307_res, A307_rcap * sizeof(uint64_t));
    }
    A307_res[A307_rcnt++] = k;
}

static a307_u128 A307_gcd(a307_u128 x, a307_u128 y)
{
    while (y) { a307_u128 t = x % y; x = y; y = t; }
    return x;
}

/* A060648(q^a) for prime q */
static a307_u128 A307_fpow(uint64_t q, unsigned a)
{
    a307_u128 qa = 1;
    for (unsigned i = 0; i < a; ++i) qa *= q;
    return (qa * q + qa - 2) / (q - 1);
}

/* ceil of the max of Prod (r+1)/(r-1) over m <= M with all primes of m below p */
static uint64_t A307_ratio(uint64_t M, uint64_t p)
{
    static const uint32_t sp[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61};
    a307_u128 num = 1, den = 1;
    uint64_t prim = 1;
    for (size_t i = 0; i < sizeof(sp) / sizeof(sp[0]); ++i) {
        uint64_t r = sp[i];
        if (r >= p || prim > M / r) break;
        prim *= r;
        num *= (r + 1);
        den *= (r - 1);
        a307_u128 g = A307_gcd(num, den);
        num /= g; den /= g;
    }
    return (uint64_t)((num + den - 1) / den);
}

/* push odd prime factors (< q) of fv = A060648(q^a) */
static void A307_push_factors(uint64_t q, unsigned a, a307_u128 fv)
{
    if (a == 1) {
        uint64_t x = q + 2;
        while (x > 1) {
            uint64_t s = A307_spf[x];
            if (s > 2 && s < q) A307_fstack[A307_ftop++] = s;
            while (x % s == 0) x /= s;
        }
    } else {
        a307_u128 x = fv;
        while ((x & 1) == 0) x >>= 1;
        for (size_t i = 1; i < A307_np && A307_pr[i] < q; ++i) {
            uint64_t r = A307_pr[i];
            if ((a307_u128)r * r > x) break;
            if (x % r == 0) {
                A307_fstack[A307_ftop++] = r;
                do x /= r; while (x % r == 0);
            }
        }
        if (x > 1 && x < q) A307_fstack[A307_ftop++] = (uint64_t)x;
    }
}

static void A307_dfs(uint64_t p, uint64_t k, a307_u128 F);

static void A307_child(uint64_t q, unsigned a, uint64_t k, a307_u128 F)
{
    uint64_t qa = 1;
    for (unsigned i = 0; i < a; ++i) qa *= q;
    a307_u128 fv = A307_fpow(q, a);
    size_t save = A307_ftop;
    A307_push_factors(q, a, fv);
    A307_dfs(q, k * qa, F * fv);
    A307_ftop = save;
}

static void A307_dfs(uint64_t p, uint64_t k, a307_u128 F)
{
    uint64_t M = A307_B / k;
    uint64_t need = k / (uint64_t)A307_gcd((a307_u128)k, F % k);
    uint64_t c = A307_ratio(M, p);
    if ((a307_u128)need > (a307_u128)M * c) return;
    if (need == 1) A307_rec(k);
    if (M < 2 || p <= 2) return;

    /* prime 2: leaf */
    {
        uint64_t kk = k;
        unsigned a = 0;
        while (kk <= A307_B / 2) {
            kk *= 2; ++a;
            a307_u128 f2 = 3 * ((a307_u128)1 << a) - 2;
            if ((F * f2) % kk == 0) A307_rec(kk);
        }
    }

    a307_u128 Mc = (a307_u128)M * c;
    size_t i;

    /* odd q not dividing F, exponent 1, q^2*need <= M*c */
    for (i = 1; i < A307_np; ++i) {
        uint64_t q = A307_pr[i];
        if (q >= p || q > M) break;
        if ((a307_u128)q * q * need > Mc) break;
        if (F % q == 0) continue;
        A307_child(q, 1, k, F);
    }

    /* q = p-2 covering p */
    if (p != A307_SENT && p >= 5 && need % p == 0) {
        uint64_t q = p - 2;
        if (q <= M && A307_spf[q] == q && F % q != 0 &&
            (a307_u128)q * q * need > Mc)
            A307_child(q, 1, k, F);
    }

    /* odd q not dividing F, exponent >= 2, q^(2a) <= M*c */
    for (i = 1; i < A307_np; ++i) {
        uint64_t q = A307_pr[i];
        if (q >= p) break;
        a307_u128 q2 = (a307_u128)q * q;
        if (q2 * q2 > Mc) break;
        if (F % q == 0) continue;
        a307_u128 qa = q2;
        unsigned a = 2;
        while (qa * qa <= Mc && qa <= M) {
            A307_child(q, a, k, F);
            qa *= q; ++a;
        }
    }

    /* odd q dividing F */
    size_t top = A307_ftop;
    for (size_t t = 0; t < top; ++t) {
        uint64_t q = A307_fstack[t];
        if (q >= p || q > M) continue;
        bool dup = false;
        for (size_t u = 0; u < t; ++u)
            if (A307_fstack[u] == q) { dup = true; break; }
        if (dup) continue;
        a307_u128 qa = q;
        unsigned a = 1;
        while (qa <= M) {
            A307_child(q, a, k, F);
            qa *= q; ++a;
        }
    }
}

static int A307_cmp(const void *x, const void *y)
{
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static uint64_t A307_isqrt(uint64_t x)
{
    uint64_t lo = 0, hi = 4294967296ULL;
    while (hi - lo > 1) {
        uint64_t mid = lo + (hi - lo) / 2;
        if ((a307_u128)mid * mid <= x) lo = mid; else hi = mid;
    }
    return lo;
}

static void A307_search(uint64_t B)
{
    A307_B = B;
    A307_rcnt = 0;
    A307_ftop = 0;
    free(A307_pr); free(A307_spf);

    uint64_t c = A307_ratio(B, A307_SENT);
    uint64_t lim = A307_isqrt(B * c) + 4;
    A307_spf = (uint32_t *)calloc((size_t)lim + 4, sizeof(uint32_t));
    A307_pr = (uint32_t *)malloc(((size_t)lim + 4) * sizeof(uint32_t));
    A307_np = 0;
    for (uint64_t x = 2; x < lim + 4; ++x) {
        if (A307_spf[x] == 0) {
            A307_spf[x] = (uint32_t)x;
            if (x <= lim) A307_pr[A307_np++] = (uint32_t)x;
            for (uint64_t j = x * x; j < lim + 4; j += x)
                if (A307_spf[j] == 0) A307_spf[j] = (uint32_t)x;
        }
    }

    A307_dfs(A307_SENT, 1, (a307_u128)1);

    qsort(A307_res, A307_rcnt, sizeof(uint64_t), A307_cmp);
    size_t w = 0;
    for (size_t r = 0; r < A307_rcnt; ++r)
        if (w == 0 || A307_res[w - 1] != A307_res[r]) A307_res[w++] = A307_res[r];
    A307_rcnt = w;
}

seq_t a(idx_t n)
{
    static uint64_t Bdone = 0;
    if (n < 1) n = 1;
    while (Bdone == 0 || A307_rcnt < n) {
        uint64_t B = Bdone ? Bdone * 8 : 65536;
        A307_search(B);
        Bdone = B;
    }
    return (seq_t)A307_res[n - 1];
}
