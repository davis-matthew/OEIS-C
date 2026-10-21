/*
 * A290309  Number of units u in Z/nZ such that Phi(5,u) is a unit, where Phi
 *          is the cyclotomic polynomial.
 * https://oeis.org/A290309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product over prime powers p^e || n of (p^(e-1) * β(p)),
 *            where β(p) = 3 if p == 5, p-5 if p ≠ 5 and 5 | (p-1), p-1
 *            otherwise
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined, but harmless */
    if (n == 1) return 1;            /* base case */

    seq_t result = 1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;

    /* factor n into primes and exponents */
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    for (size_t i = 0; i < pcnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];

        /* p^(e-1) */
        seq_t powp = 1;
        for (uint64_t k = 1; k < e; ++k)
            powp *= p;

        seq_t beta;
        if (primes[i] == 5ULL) {
            beta = (seq_t)3;                      /* 5 has 1 bad residue (1 mod 5) */
        } else if (((primes[i] - 1ULL) % 5ULL) == 0ULL) {
            beta = (seq_t)(primes[i] - 5ULL);      /* drop the 4 primitive 5‑th roots */
        } else {
            beta = (seq_t)(primes[i] - 1ULL);      /* all units survive */
        }

        result *= powp * beta;
    }

    return result;
}
