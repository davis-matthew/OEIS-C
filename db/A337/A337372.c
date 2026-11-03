/*
 * A337372  Primitively primeshift-abundant numbers: Numbers that are included
 *          in A246282 (k with A003961(k) > 2k), but none of whose proper
 *          divisors are.
 * https://oeis.org/A337372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 665, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 80 terms through n = 9441 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A337372: k with A003961(k) > 2k (A003961 replacing each prime by the next one)
 * none of whose proper divisors has that property.
 * f(k) = A003961(k) satisfies f(k)/k = Prod (nextprime(p)/p)^e, every factor
 * above 1, so the property passes to multiples: k is a term iff f(k) > 2k and
 * f(k/p) <= 2k/p for each prime p | k. A smallest-prime-factor sieve up to a
 * limit L (doubling as needed) gives the factorizations; f is exact in
 * 128 bits. (The original lost its record of earlier terms when its sieve
 * grew.) */

#include <stdint.h>
#include <stdlib.h>

typedef unsigned __int128 u128;

static uint64_t *terms = NULL;
static size_t term_len = 0;

static void build(uint64_t L)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)L + 2, sizeof *spf);
    uint32_t *nxt = (uint32_t *)calloc((size_t)L + 2, sizeof *nxt);   /* nextprime of a prime */
    if (!spf || !nxt) abort();
    for (uint64_t i = 2; i <= L; ++i)
        if (!spf[i])
            for (uint64_t j = i; j <= L; j += i)
                if (!spf[j]) spf[j] = (uint32_t)i;
    /* nextprime(p) for primes p <= L; the one after the last prime below L
       is found by trial division */
    uint64_t last = 0;
    for (uint64_t i = 2; i <= L; ++i)
        if (spf[i] == i) {
            if (last) nxt[last] = (uint32_t)i;
            last = i;
        }
    for (uint64_t q = last + 1;; ++q) {
        int prime = 1;
        for (uint64_t d = 2; d * d <= q; ++d) if (q % d == 0) { prime = 0; break; }
        if (prime) { nxt[last] = (uint32_t)q; break; }
    }
    size_t cnt = 0, cap = 1024;
    uint64_t *t = (uint64_t *)malloc(cap * sizeof *t);
    if (!t) abort();
    for (uint64_t k = 2; k <= L; ++k) {
        uint64_t P[16], x = k;
        int np = 0;
        u128 f = 1;
        while (x > 1) {
            uint64_t p = spf[x];
            x /= p;
            f *= nxt[p];
            if (!np || P[np - 1] != p) P[np++] = p;
        }
        if (!(f > (u128)2 * k)) continue;
        int primitive = 1;
        for (int i = 0; i < np && primitive; ++i) {
            u128 g = f / nxt[P[i]];                  /* f(k/p) = f(k) / nextprime(p) */
            if (g > (u128)2 * (k / P[i])) primitive = 0;
        }
        if (!primitive) continue;
        if (cnt == cap) {
            uint64_t *u = (uint64_t *)realloc(t, (cap *= 2) * sizeof *t);
            if (!u) abort();
            t = u;
        }
        t[cnt++] = k;
    }
    free(spf);
    free(nxt);
    free(terms);
    terms = t;
    term_len = cnt;
}

seq_t a(idx_t n)
{
    static uint64_t L = 0;
    if (n < 1) return (seq_t)0;
    while (term_len < (size_t)n) {
        L = L ? 2 * L : 1 << 20;
        if (L > (UINT64_C(1) << 28)) return (seq_t)0;
        build(L);
    }
    return (seq_t)terms[n - 1];
}
