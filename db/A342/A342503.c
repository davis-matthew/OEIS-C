/*
 * A342503  Numbers k such that k divides A340740(k).
 * https://oeis.org/A342503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   sieve. Möbius inversion gives A340740(k) = Sum_{d|k}
 *            mu(d)*d*G(k/d), where G(q) = Sum_{m<=floor((q-1)/2)} (q mod m),
 *            and G(q) == -(Sum_{i<=q} sigma(i) + c*(c-1)/2) (mod q) with c =
 *            ceil(q/2). So: sieve sigma, take its prefix sums mod q, apply
 *            the Dirichlet factor (1 - p) for each prime p in place...
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A342503_terms = NULL;
static size_t A342503_nterms = 0;
static uint64_t A342503_limit = 0;

static void A342503_build(uint64_t N)
{
    uint32_t *f = (uint32_t *)malloc((size_t)(N + 1) * sizeof(uint32_t));
    size_t pcap = 1024, pcnt = 0;
    uint32_t *pr = (uint32_t *)malloc(pcap * sizeof(uint32_t));
    uint64_t k, j, p;
    f[0] = 0;
    for (k = 1; k <= N; ++k) f[k] = (uint32_t)k;
    /* sigma by in-place zeta transform over primes; primes detected as untouched entries */
    for (p = 2; p <= N; ++p) {
        if (f[p] != (uint32_t)p) continue;
        if (pcnt == pcap) { pcap *= 2; pr = (uint32_t *)realloc(pr, pcap * sizeof(uint32_t)); }
        pr[pcnt++] = (uint32_t)p;
        uint64_t lim = N / p;
        for (j = 1; j <= lim; ++j) f[j * p] += f[j];
    }
    /* g(q) = G(q) mod q */
    {
        uint64_t S = 0;
        for (k = 1; k <= N; ++k) {
            S += f[k];
            uint64_t c = (k + 1) / 2;
            uint64_t R = c * (c - 1) / 2;
            uint64_t r = (S + R) % k;
            f[k] = (uint32_t)((k - r) % k);
        }
    }
    /* Dirichlet convolution with mu(d)*d, prime by prime, values kept mod index */
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t P = pr[i];
        for (j = N / P; j >= 1; --j) {
            uint64_t kk = j * P;
            uint64_t sub = P * (uint64_t)f[j];      /* < kk since f[j] < j */
            uint64_t cur = f[kk];
            f[kk] = (uint32_t)(cur >= sub ? cur - sub : cur + kk - sub);
        }
    }
    size_t cnt = 0;
    for (k = 1; k <= N; ++k) if (f[k] == 0) ++cnt;
    uint64_t *t = (uint64_t *)malloc((cnt + 1) * sizeof(uint64_t));
    cnt = 0;
    for (k = 1; k <= N; ++k) if (f[k] == 0) t[cnt++] = k;
    free(f);
    free(pr);
    if (A342503_terms) free(A342503_terms);
    A342503_terms = t;
    A342503_nterms = cnt;
    A342503_limit = N;
}

seq_t a(idx_t n)
{
    while (A342503_nterms < n) {
        uint64_t N = A342503_limit ? A342503_limit + A342503_limit / 2 : ((uint64_t)1 << 20);
        A342503_build(N);
    }
    return (seq_t)A342503_terms[n - 1];
}
