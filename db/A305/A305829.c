/*
 * A305829  Factor n into distinct Fermi-Dirac primes (A050376), normalize by
 *          replacing every instance of the k-th Fermi-Dirac prime with k,
 *          then multiply everything together.
 * https://oeis.org/A305829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product of the indices of the Fermi‑Dirac primes whose
 *            values multiply to n (sequence A305829)
 * Range:     u8 n <= 854, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 87 terms through n = 960 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A305829: factor n into distinct Fermi-Dirac primes (the p^(2^b), A050376:
 * 2, 3, 4, 5, 7, 9, ...), one for each binary digit b of each exponent, and
 * multiply their indices. The index of q = p^(2^b) in A050376 is the number of
 * them up to q, Sum_{c>=0} pi(floor(q^(1/2^c))); pi comes from a sieve up to n
 * that grows as needed. */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *pi = NULL;
static uint64_t lim = 0;

static void build(uint64_t L)
{
    unsigned char *c = (unsigned char *)calloc((size_t)L + 1, 1);
    uint32_t *p = (uint32_t *)malloc(((size_t)L + 1) * sizeof *p);
    if (!c || !p) abort();
    for (uint64_t i = 2; i * i <= L; ++i)
        if (!c[i]) for (uint64_t j = i * i; j <= L; j += i) c[j] = 1;
    p[0] = p[1] = 0;
    for (uint64_t i = 2; i <= L; ++i) p[i] = p[i - 1] + !c[i];
    free(c);
    free(pi);
    pi = p;
    lim = L;
}

static uint64_t isqrt64(uint64_t m)
{
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= m) ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    const uint64_t N = (uint64_t)n;
    if (N > lim) {
        uint64_t L = lim ? 2 * lim : 1 << 16;
        while (L < N) L *= 2;
        build(L);
    }
    uint64_t t = N, prod = 1;
    for (uint64_t p = 2; t > 1; ++p) {
        if (p * p > t) p = t;                     /* t is prime */
        if (t % p) continue;
        unsigned e = 0;
        while (t % p == 0) { t /= p; ++e; }
        uint64_t q = p;                           /* p^(2^b) */
        for (unsigned b = 0; e >> b; ++b, q = q * q) {
            if (!((e >> b) & 1)) continue;
            uint64_t idx = 0;
            for (uint64_t r = q; r >= 2; r = isqrt64(r)) idx += pi[r];
            prod *= idx;
            if (e >> (b + 1) == 0) break;
        }
    }
    return (seq_t)prod;
}
