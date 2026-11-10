/*
 * A305832  Number of connected components of the n-th FDH set-system.
 * https://oeis.org/A305832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: FD-factor n = f(s_1)*...*f(s_k) with f = A050376,
 *            FD-factor each index s_i into an edge, and count the connected
 *            components of these edges with union-find
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

static unsigned char *A305832_comp = NULL; /* 1 = composite */
static uint64_t A305832_lim = 0;

static void A305832_sieve(uint64_t lim) {
    if (lim <= A305832_lim) return;
    uint64_t L = lim < 1024 ? 1024 : lim * 2;
    if (A305832_comp) free(A305832_comp);
    A305832_comp = (unsigned char *)calloc((size_t)(L + 1), 1);
    A305832_comp[0] = A305832_comp[1] = 1;
    for (uint64_t i = 2; i * i <= L; i++)
        if (!A305832_comp[i])
            for (uint64_t j = i * i; j <= L; j += i) A305832_comp[j] = 1;
    A305832_lim = L;
}

static uint64_t A305832_pi(uint64_t x) {
    uint64_t c = 0;
    for (uint64_t i = 2; i <= x; i++) if (!A305832_comp[i]) c++;
    return c;
}

static uint64_t A305832_isqrt(uint64_t x) {
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= x) r++;
    return r;
}

/* Fermi-Dirac prime factors (values p^(2^k)) of m */
static int A305832_fd(uint64_t m, uint64_t *out) {
    int c = 0;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p) continue;
        int e = 0;
        while (m % p == 0) { m /= p; e++; }
        uint64_t q = p;
        while (e) {
            if (e & 1) out[c++] = q;
            e >>= 1;
            if (e) q = q * q;
        }
    }
    if (m > 1) out[c++] = m;
    return c;
}

/* index of Fermi-Dirac prime q in A050376 */
static uint64_t A305832_rank(uint64_t q) {
    uint64_t total = 0, r = q;
    while (r >= 2) {
        total += A305832_pi(r);
        r = A305832_isqrt(r);
    }
    return total;
}

static int A305832_find(int *par, int x) {
    while (par[x] != x) { par[x] = par[par[x]]; x = par[x]; }
    return x;
}

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    A305832_sieve((uint64_t)n);
    uint64_t fs[70];
    int k = A305832_fd((uint64_t)n, fs);
    static uint64_t ev[70][70];
    int ec[70];
    int par[70];
    for (int i = 0; i < k; i++) {
        uint64_t s = A305832_rank(fs[i]);
        ec[i] = A305832_fd(s, ev[i]);
        par[i] = i;
    }
    for (int i = 0; i < k; i++)
        for (int j = i + 1; j < k; j++) {
            bool share = false;
            for (int x = 0; x < ec[i] && !share; x++)
                for (int y = 0; y < ec[j]; y++)
                    if (ev[i][x] == ev[j][y]) { share = true; break; }
            if (share) {
                int ri = A305832_find(par, i), rj = A305832_find(par, j);
                if (ri != rj) par[ri] = rj;
            }
        }
    int comps = 0;
    for (int i = 0; i < k; i++)
        if (A305832_find(par, i) == i) comps++;
    return (seq_t)comps;
}
