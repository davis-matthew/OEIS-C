/*
 * A305831  Number of connected components of the strict integer partition
 *          with FDH number n.
 * https://oeis.org/A305831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of connected components of the graph on the
 *            FD-indices {k : f(k) | n in the Fermi-Dirac factorization of n},
 *            f = A050376, with an edge when gcd(k_i, k_j) > 1. Index of
 *            p^(2^j) = Sum_{i>=0} pi(floor(q^(1/2^i)))
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint32_t *A305831_pi = NULL;   /* prefix prime counts */
static uint64_t A305831_lim = 0;

static void A305831_ensure(uint64_t lim) {
    if (lim < 16) lim = 16;
    if (lim <= A305831_lim) return;
    uint64_t L = lim * 2;
    unsigned char *comp = (unsigned char *)calloc((size_t)(L + 1), 1);
    uint32_t *pi = (uint32_t *)malloc((size_t)(L + 1) * sizeof(uint32_t));
    comp[0] = comp[1] = 1;
    for (uint64_t i = 2; i * i <= L; i++)
        if (!comp[i])
            for (uint64_t j = i * i; j <= L; j += i) comp[j] = 1;
    uint32_t c = 0;
    for (uint64_t i = 0; i <= L; i++) {
        if (!comp[i]) c++;
        pi[i] = c;
    }
    free(comp);
    free(A305831_pi);
    A305831_pi = pi;
    A305831_lim = L;
}

static uint64_t A305831_isqrt(uint64_t x) {
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= x) r++;
    return r;
}

static uint64_t A305831_gcd(uint64_t a, uint64_t b) {
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static int A305831_find(int *par, int x) {
    while (par[x] != x) {
        par[x] = par[par[x]];
        x = par[x];
    }
    return x;
}

/* index k with f(k) = q: count of FD primes <= q */
static uint64_t A305831_fd_index(uint64_t q) {
    uint64_t total = 0;
    uint64_t r = q;
    while (r >= 2) {
        total += A305831_pi[r];
        r = A305831_isqrt(r);
    }
    return total;
}

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    uint64_t N = (uint64_t)n;
    A305831_ensure(N);
    uint64_t ks[80];
    int cnt = 0;
    uint64_t m = N;
    uint64_t p = 2;
    while (m > 1) {
        if (p * p > m) p = m;
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) { m /= p; e++; }
            uint64_t q = p;
            while (e) {
                if (e & 1) ks[cnt++] = A305831_fd_index(q);
                e >>= 1;
                if (e) q = q * q;
            }
        }
        p += (p == 2) ? 1 : 2;
    }
    int par[80];
    for (int i = 0; i < cnt; i++) par[i] = i;
    for (int i = 0; i < cnt; i++)
        for (int j = i + 1; j < cnt; j++)
            if (A305831_gcd(ks[i], ks[j]) > 1) {
                int x = A305831_find(par, i), y = A305831_find(par, j);
                if (x != y) par[x] = y;
            }
    int comps = 0;
    for (int i = 0; i < cnt; i++)
        if (A305831_find(par, i) == i) comps++;
    return (seq_t)comps;
}
