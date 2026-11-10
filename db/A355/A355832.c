/*
 * A355832  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A354347(i) = A354347(j) for all i, j >= 1.
 * https://oeis.org/A355832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = restricted growth sequence transform of A354347, the
 *            Dirichlet inverse of A345000(n) = gcd(A003415(n),
 *            A003415(A276086(n))); computed by a Dirichlet-inverse sieve
 *            (values kept as two modular hashes) and first-occurrence class
 *            numbering
 * Range:     u8 n <= 1460, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9932 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

typedef unsigned __int128 u128_t;

static uint64_t A355832_deriv(uint64_t n)
{
    if (n <= 1) return 0;
    uint64_t res = 0, t = n;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            uint64_t e = 0;
            while (t % p == 0) { ++e; t /= p; }
            res += e * (n / p);
        }
    }
    if (t > 1) res += n / t;
    return res;
}

static uint64_t A355832_mm(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)(((u128_t)a * b) % m);
}

static uint64_t A355832_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static uint64_t A355832_f(uint64_t n)
{
    static const uint64_t pr[25] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
    if (n <= 1) return 1;
    uint64_t M = A355832_deriv(n);
    if (M == 1) return 1;
    unsigned e[25];
    int k = 0;
    uint64_t m = n;
    while (m > 0 && k < 25) {
        e[k] = (unsigned)(m % pr[k]);
        m /= pr[k];
        ++k;
    }
    uint64_t pw[25];
    for (int i = 0; i < k; ++i) {
        uint64_t r = 1 % M;
        for (unsigned j = 0; j < e[i]; ++j) r = A355832_mm(r, pr[i] % M, M);
        pw[i] = r;
    }
    uint64_t S = 0;
    for (int i = 0; i < k; ++i) {
        if (e[i] == 0) continue;
        uint64_t t = 1 % M;
        for (int j = 0; j < k; ++j) {
            if (j == i) continue;
            t = A355832_mm(t, pw[j], M);
        }
        uint64_t q = 1 % M;
        for (unsigned j = 0; j + 1 < e[i]; ++j) q = A355832_mm(q, pr[i] % M, M);
        t = A355832_mm(t, q, M);
        t = A355832_mm(t, (uint64_t)e[i] % M, M);
        S = (S + t) % M;
    }
    return A355832_gcd(M, S);
}

static const uint64_t A355832_P = (((uint64_t)1) << 61) - 1;

static uint64_t *A355832_h1 = NULL;
static uint64_t *A355832_h2 = NULL;

static int A355832_cmp(const void *x, const void *y)
{
    size_t i = *(const size_t *)x, j = *(const size_t *)y;
    if (A355832_h1[i] != A355832_h1[j]) return A355832_h1[i] < A355832_h1[j] ? -1 : 1;
    if (A355832_h2[i] != A355832_h2[j]) return A355832_h2[i] < A355832_h2[j] ? -1 : 1;
    if (i != j) return i < j ? -1 : 1;
    return 0;
}

static size_t A355832_N = 0;
static uint64_t *A355832_res = NULL;

static void A355832_build(size_t N)
{
    uint64_t *f = (uint64_t *)malloc((N + 1) * sizeof(uint64_t));
    uint64_t *a1 = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    uint64_t *a2 = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    size_t *idx = (size_t *)malloc(N * sizeof(size_t));
    uint64_t *rep = (uint64_t *)malloc((N + 1) * sizeof(uint64_t));
    uint64_t *res = (uint64_t *)malloc((N + 1) * sizeof(uint64_t));
    for (size_t d = 1; d <= N; ++d) f[d] = A355832_f(d);
    a1[1] = 1; a2[1] = 1;
    for (size_t i = 1; i <= N; ++i) {
        uint64_t b1 = a1[i], b2 = a2[i];
        if (b1 == 0 && b2 == 0) continue;
        for (size_t d = 2; i * d <= N; ++d) {
            size_t n = i * d;
            a1[n] -= f[d] * b1;
            uint64_t t = (uint64_t)(((u128_t)(f[d] % A355832_P) * b2) % A355832_P);
            a2[n] = (a2[n] + A355832_P - t) % A355832_P;
        }
    }
    A355832_h1 = a1; A355832_h2 = a2;
    for (size_t i = 0; i < N; ++i) idx[i] = i + 1;
    qsort(idx, N, sizeof(size_t), A355832_cmp);
    for (size_t i = 0; i < N; ) {
        size_t j = i;
        while (j < N && a1[idx[j]] == a1[idx[i]] && a2[idx[j]] == a2[idx[i]]) ++j;
        for (size_t t = i; t < j; ++t) rep[idx[t]] = idx[i];
        i = j;
    }
    uint64_t id = 0;
    for (size_t n = 1; n <= N; ++n) {
        if (rep[n] == n) res[n] = ++id;
        else res[n] = res[rep[n]];
    }
    free(f); free(a1); free(a2); free(idx); free(rep);
    A355832_h1 = NULL; A355832_h2 = NULL;
    free(A355832_res);
    A355832_res = res;
    A355832_N = N;
}

seq_t a(idx_t n)
{
    if ((size_t)n > A355832_N) {
        size_t N = (size_t)n;
        if (N < 2 * A355832_N) N = 2 * A355832_N;
        if (N < 1000) N = 1000;
        A355832_build(N);
    }
    return (seq_t)A355832_res[n];
}
