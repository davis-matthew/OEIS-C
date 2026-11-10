/*
 * A353633  a(n) = 1 if A351546(n) is a unitary divisor of n, otherwise 0.
 *          Here A351546(n) is the largest unitary divisor of sigma(n) coprime
 *          with A003961(n).
 * https://oeis.org/A353633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if u = A351546(n) is a unitary divisor of n (u | n and
 *            gcd(u, n/u) = 1), else 0; u = product of q^f || sigma(n) over
 *            primes q not dividing A003961(n)
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 136 terms through n = 9804 (2026-11-10)
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

static uint64_t A353633_gcd(uint64_t x, uint64_t y) {
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    uint64_t nn = (uint64_t)n;
    uint64_t primes[32], exps[32];
    size_t pcnt;
    factor_u64(nn, primes, exps, &pcnt);

    uint64_t nextp[32];
    uint64_t sigma = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        nextp[i] = next_prime(p);
        uint64_t term = 1, pw = 1;
        for (uint64_t k = 0; k < exps[i]; ++k) {
            pw *= p;
            term += pw;
        }
        sigma *= term;
    }

    uint64_t sp[32], se[32];
    size_t scnt;
    factor_u64(sigma, sp, se, &scnt);

    uint64_t u = 1;
    for (size_t i = 0; i < scnt; ++i) {
        uint64_t q = sp[i];
        bool excl = false;
        for (size_t j = 0; j < pcnt; ++j)
            if (nextp[j] == q) { excl = true; break; }
        if (!excl) {
            for (uint64_t k = 0; k < se[i]; ++k) u *= q;
            if (u > nn) return 0;
        }
    }

    if (nn % u != 0) return 0;
    return (A353633_gcd(u, nn / u) == 1) ? 1 : 0;
}
