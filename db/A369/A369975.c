/*
 * A369975  Parity of A369974.
 * https://oeis.org/A369975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A369978(n) mod 2, where A369978 is the Dirichlet inverse
 *            of 1+A083345(n). Mod 2: a(1)=1, a(n) = XOR over d|n, d<n of
 *            b(n/d)&a(d), with b(m) = 1 if m'/gcd(m,m') is even (b =
 *            A369001), else 0. Computed by a sieve over divisors
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 137 terms through n = 9557 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static unsigned char *par_a = NULL;
static idx_t par_N = 0;

/* b(m) = 1 iff m'/gcd(m,m') is even, i.e. v2(m') > v2(m) (m=1: m'=0 -> 1) */
static unsigned char a369975_b(uint64_t m)
{
    if (m == 1) return 1;
    unsigned __int128 der = 0;
    uint64_t t = m;
    for (uint64_t p = 2; p * p <= t; p += (p == 2 ? 1 : 2)) {
        if (t % p == 0) {
            unsigned e = 0;
            while (t % p == 0) { t /= p; ++e; }
            der += (unsigned __int128)(m / p) * e;
        }
    }
    if (t > 1) der += (unsigned __int128)(m / t);
    int v2m = 0;
    uint64_t u = m;
    while ((u & 1) == 0) { u >>= 1; ++v2m; }
    if (der == 0) return 1;
    int v2d = 0;
    while ((der & 1) == 0) { der >>= 1; ++v2d; }
    return (unsigned char)(v2d > v2m ? 1 : 0);
}

static void a369975_build(idx_t N)
{
    free(par_a);
    size_t sz = (size_t)N + 1;
    unsigned char *acc = (unsigned char *)calloc(sz, 1);
    unsigned char *b = (unsigned char *)calloc(sz, 1);
    if (!acc || !b) abort();
    for (size_t m = 1; m < sz; ++m) b[m] = a369975_b((uint64_t)m);
    acc[1] = 1;
    for (size_t d = 1; d < sz; ++d) {
        if (d > 1 && 0) {}
        unsigned char ad = acc[d];
        if (!ad) continue;
        for (size_t m = 2 * d, q = 2; m < sz; m += d, ++q)
            acc[m] ^= b[q];
    }
    free(b);
    par_a = acc;
    par_N = N;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n > par_N || !par_a) {
        idx_t N = par_N ? par_N * 2 : 1024;
        if (N < n) N = n;
        a369975_build(N);
    }
    return (seq_t)(par_a[n] & 1);
}
