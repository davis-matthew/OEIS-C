/*
 * A332552  a(n) = A082184(n) - A082183(n).
 * https://oeis.org/A332552
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A082184(n) - A082183(n) = u, the largest divisor u < n of
 *            n(n+1) with u + n(n+1)/u odd (and n(n+1)/u - u >= 3); computed
 *            by divisor enumeration
 * Range:     u8 n <= 295, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9429 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

typedef unsigned __int128 A332552_u128;

static int A332552_factor(uint64_t x, uint64_t *P, int *E, int np)
{
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p) continue;
        int e = 0;
        while (x % p == 0) { x /= p; ++e; }
        int i = 0;
        while (i < np && P[i] != p) ++i;
        if (i == np) { P[np] = p; E[np++] = 0; }
        E[i] += e;
    }
    if (x > 1) {
        int i = 0;
        while (i < np && P[i] != x) ++i;
        if (i == np) { P[np] = x; E[np++] = 0; }
        E[i] += 1;
    }
    return np;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    const uint64_t N = (uint64_t)n;
    uint64_t P[64];
    int E[64];
    int np = A332552_factor(N, P, E, 0);
    np = A332552_factor(N + 1, P, E, np);
    const A332552_u128 M = (A332552_u128)N * (N + 1);
    size_t cnt = 1;
    for (int i = 0; i < np; ++i) cnt *= (size_t)(E[i] + 1);
    uint64_t *d = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!d) abort();
    size_t nd = 1;
    d[0] = 1;
    for (int i = 0; i < np; ++i) {
        size_t old = nd;
        A332552_u128 m = 1;
        for (int e = 1; e <= E[i]; ++e) {
            m *= P[i];
            for (size_t j = 0; j < old; ++j) {
                A332552_u128 v = (A332552_u128)d[j] * m;
                if (v < N) d[nd++] = (uint64_t)v;
            }
        }
    }
    uint64_t best = 0;
    for (size_t j = 0; j < nd; ++j) {
        A332552_u128 u = d[j], v = M / u;
        if (((u + v) & 1) && v - u >= 3 && u > best) best = (uint64_t)u;
    }
    free(d);
    return (seq_t)best;
}
