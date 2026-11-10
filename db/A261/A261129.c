/*
 * A261129  Highest exponent in prime factorization of the swinging factorial
 *          (A056040).
 * https://oeis.org/A261129
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max over primes p<=n of (v_p(n!) - 2*v_p(floor(n/2)!)),
 *            the highest exponent in the factorization of n!/floor(n/2)!^2
 *            (A056040), via Legendre's formula; v_p = p-adic valuation
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 8990 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t A261129_legendre(uint64_t n, uint64_t p)
{
    uint64_t s = 0;
    while (n) {
        n /= p;
        s += n;
    }
    return s;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    uint64_t N = (uint64_t)n;
    uint64_t h = N / 2;
    unsigned char *comp = (unsigned char *)calloc((size_t)N + 1, 1);
    uint64_t best = 0;

    for (uint64_t p = 2; p <= N; ++p) {
        if (comp[p])
            continue;
        if (p <= N / p) {
            for (uint64_t q = p * p; q <= N; q += p)
                comp[q] = 1;
        }
        uint64_t e = A261129_legendre(N, p) - 2 * A261129_legendre(h, p);
        if (e > best)
            best = e;
    }

    free(comp);
    return (seq_t)best;
}
