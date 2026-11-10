/*
 * A344700  Numbers k for which A306927(k) [= A001615(k)-k] is a multiple of
 *          A344705(k) [= A001615(k)-A001065(k)], and their quotient is
 *          nonnegative.
 * https://oeis.org/A344700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. a(n) is the n-th k such that B = k +
 *            psi(k) - sigma(k) > 0 and B divides A = psi(k) - k, where psi is
 *            A001615 and sigma(k) - k is A001065. Method: depth-first search
 *            over factorizations. Every k <= X is either a node m (built from
 *            primes p with m*p^2 <= X) or m*p with p...
 * Range:     u8 n <= 5, u16 n <= 55, u32 n <= 782, u64 n <= 782,
 *            u128 n <= 782, big any n
 * Validated: standard, pass, 67 terms through n = 618 (2026-11-10)
 * Helpers:   is_prime_u64, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"
#include "../utils/isqrt_u64.h"

typedef __int128 A344700_i128;

static uint64_t A344700_X = 0;
static uint64_t *A344700_terms = NULL;
static size_t A344700_cnt = 0, A344700_cap = 0;
static uint32_t *A344700_pr = NULL;
static size_t A344700_npr = 0;

static void A344700_add(uint64_t v)
{
    if (A344700_cnt == A344700_cap) {
        A344700_cap = A344700_cap ? A344700_cap * 2 : 1024;
        A344700_terms = (uint64_t *)realloc(A344700_terms, sizeof(uint64_t) * A344700_cap);
    }
    A344700_terms[A344700_cnt++] = v;
}

static void A344700_sieve(uint64_t lim)
{
    free(A344700_pr);
    char *c = (char *)calloc((size_t)lim + 1, 1);
    A344700_pr = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)lim / 2 + 2));
    A344700_npr = 0;
    for (uint64_t i = 2; i <= lim; i++) {
        if (!c[i]) {
            A344700_pr[A344700_npr++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= lim; j += i) c[j] = 1;
        }
    }
    free(c);
}

/* n = m*p, p prime > P(m): check the condition directly */
static void A344700_check_mp(uint64_t m, uint64_t S, uint64_t T, uint64_t p)
{
    A344700_i128 E = (A344700_i128)m + T - S;
    A344700_i128 A = (A344700_i128)T * (p + 1) - (A344700_i128)m * p;
    A344700_i128 B = (A344700_i128)p * E + ((A344700_i128)T - (A344700_i128)S);
    if (B > 0 && A >= 0 && A % B == 0) A344700_add(m * p);
}

static void A344700_leaf(uint64_t m, uint64_t S, uint64_t T, uint64_t P, uint64_t s)
{
    A344700_i128 E = (A344700_i128)m + T - S;
    if (E <= 0) return;
    uint64_t pl = P + 1;
    if (s + 1 > pl) pl = s + 1;
    uint64_t pu = A344700_X / m;
    if (pl > pu) return;
    A344700_i128 TM = (A344700_i128)T - m;
    A344700_i128 ST = (A344700_i128)S - T;
    A344700_i128 qlo = TM / E + 1;
    if (qlo < 1) qlo = 1;
    A344700_i128 d2 = (A344700_i128)pu * E - ST;
    if (d2 > 0) {
        A344700_i128 N = (A344700_i128)T + (A344700_i128)pu * TM;
        A344700_i128 c = (N + d2 - 1) / d2;
        if (c > qlo) qlo = c;
    }
    A344700_i128 np = (A344700_i128)(pu - pl + 1);
    A344700_i128 d1 = (A344700_i128)pl * E - ST;
    if (d1 > 0) {
        A344700_i128 qhi = ((A344700_i128)T + (A344700_i128)pl * TM) / d1;
        A344700_i128 nq = qhi >= qlo ? qhi - qlo + 1 : 0;
        if (nq <= np) {
            for (A344700_i128 q = qlo; q <= qhi; q++) {
                A344700_i128 den = q * E - TM;
                if (den <= 0) continue;
                A344700_i128 D = (A344700_i128)T + q * ST;
                if (D % den != 0) continue;
                A344700_i128 pp = D / den;
                if (pp < (A344700_i128)pl || pp > (A344700_i128)pu) continue;
                uint64_t p = (uint64_t)pp;
                if (is_prime_u64(p)) A344700_check_mp(m, S, T, p);
            }
            return;
        }
    }
    for (uint64_t p = pl; p <= pu; p++) {
        A344700_i128 A = (A344700_i128)T * (p + 1) - (A344700_i128)m * p;
        A344700_i128 B = (A344700_i128)p * E - ST;
        if (B > 0 && A >= 0 && A % B == 0 && is_prime_u64(p)) A344700_add(m * p);
    }
}

static void A344700_dfs(uint64_t m, uint64_t S, uint64_t T, uint64_t P, size_t idx)
{
    A344700_i128 A = (A344700_i128)T - m;
    A344700_i128 B = (A344700_i128)m + T - S;
    if (B > 0 && A >= 0 && A % B == 0) A344700_add(m);

    uint64_t lim = A344700_X / m;
    uint64_t s = isqrt_u64(lim);
    while ((s + 1) * (s + 1) <= lim) s++;
    while (s * s > lim) s--;

    A344700_leaf(m, S, T, P, s);

    for (size_t k = idx; k < A344700_npr; k++) {
        uint64_t p = A344700_pr[k];
        if (p > s) break;
        uint64_t pe = p, sg = 1 + p, ps = p + 1;
        for (;;) {
            A344700_dfs(m * pe, S * sg, T * ps, p, k + 1);
            if (pe > lim / p) break;
            pe *= p;
            sg += pe;
            ps *= p;
        }
    }
}

static int A344700_cmp(const void *x, const void *y)
{
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static void A344700_run(uint64_t X)
{
    A344700_X = X;
    A344700_cnt = 0;
    uint64_t r = isqrt_u64(X);
    A344700_sieve(r + 2);
    A344700_dfs(1, 1, 1, 1, 0);
    qsort(A344700_terms, A344700_cnt, sizeof(uint64_t), A344700_cmp);
}

seq_t a(idx_t n)
{
    if (A344700_X == 0) A344700_run((uint64_t)1 << 16);
    while (A344700_cnt < (size_t)n) A344700_run(A344700_X * 4);
    return (seq_t)A344700_terms[n - 1];
}
