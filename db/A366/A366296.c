/*
 * A366296  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A346242(i) = A346242(j) for all i, j >= 1, where A346242 is
 *          Dirichlet inverse of gcd(n, A276086(n)).
 * https://oeis.org/A366296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = restricted growth sequence transform of A346242, where
 *            A346242 is the Dirichlet inverse of gcd(n, A276086(n)). Computed
 *            by a divisor sieve, with ranks assigned by first occurrence
 * Range:     u8 n <= 2645, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 112 terms through n = 9504 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

typedef __int128 a366296_val;

typedef struct {
    a366296_val v;
    uint64_t i;
} a366296_pair;

static int a366296_cmp(const void *x, const void *y)
{
    const a366296_pair *p = (const a366296_pair *)x;
    const a366296_pair *q = (const a366296_pair *)y;
    if (p->v < q->v) return -1;
    if (p->v > q->v) return 1;
    if (p->i < q->i) return -1;
    if (p->i > q->i) return 1;
    return 0;
}

/* gcd(m, A276086(m)) computed through the primorial-base digits of m */
static uint64_t a366296_f(uint64_t m)
{
    static const uint64_t P[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
    uint64_t rem = m, res = 1;
    for (int k = 0; k < 16 && rem > 0; ++k) {
        uint64_t p = P[k];
        uint64_t d = rem % p;
        rem /= p;
        if (d == 0) continue;
        uint64_t t = m, e = 0;
        while (e < d && t % p == 0) { t /= p; ++e; }
        for (uint64_t j = 0; j < e; ++j) res *= p;
    }
    return res;
}

static uint64_t *a366296_lab = NULL;
static uint64_t a366296_N = 0;

static void a366296_build(uint64_t N)
{
    a366296_val *g = (a366296_val *)calloc((size_t)N + 1, sizeof(a366296_val));
    a366296_val *acc = (a366296_val *)calloc((size_t)N + 1, sizeof(a366296_val));
    uint64_t *f = (uint64_t *)calloc((size_t)N + 1, sizeof(uint64_t));
    a366296_pair *pr = (a366296_pair *)malloc(((size_t)N + 1) * sizeof(a366296_pair));
    uint64_t *lead = (uint64_t *)calloc((size_t)N + 1, sizeof(uint64_t));
    uint64_t *lab = (uint64_t *)calloc((size_t)N + 1, sizeof(uint64_t));
    if (!g || !acc || !f || !pr || !lead || !lab) abort();

    for (uint64_t m = 1; m <= N; ++m) f[m] = a366296_f(m);

    for (uint64_t i = 1; i <= N; ++i) {
        g[i] = (i == 1) ? (a366296_val)1 : -acc[i];
        for (uint64_t j = 2 * i, q = 2; j <= N; j += i, ++q)
            acc[j] += g[i] * (a366296_val)f[q];
    }

    for (uint64_t i = 1; i <= N; ++i) {
        pr[i - 1].v = g[i];
        pr[i - 1].i = i;
    }
    qsort(pr, (size_t)N, sizeof(a366296_pair), a366296_cmp);
    for (uint64_t k = 0; k < N; ++k) {
        if (k > 0 && pr[k].v == pr[k - 1].v)
            lead[pr[k].i] = lead[pr[k - 1].i];
        else
            lead[pr[k].i] = pr[k].i;
    }
    uint64_t cnt = 0;
    for (uint64_t i = 1; i <= N; ++i) {
        if (lead[i] == i) lab[i] = ++cnt;
        else lab[i] = lab[lead[i]];
    }

    free(g); free(acc); free(f); free(pr); free(lead);
    free(a366296_lab);
    a366296_lab = lab;
    a366296_N = N;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if ((uint64_t)n > a366296_N) {
        uint64_t N = (uint64_t)n;
        if (N < 2 * a366296_N) N = 2 * a366296_N;
        if (N < 1024) N = 1024;
        a366296_build(N);
    }
    return (seq_t)a366296_lab[(uint64_t)n];
}
