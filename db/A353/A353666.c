/*
 * A353666  a(n) = gcd(n, A351546(n)).
 * https://oeis.org/A353666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, A351546(n)), where A351546(n) is the largest
 *            unitary divisor of sigma(n) coprime to A003961(n)
 * Range:     u8 n <= 37, u16 n <= 8389, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9768 (2026-11-10)
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

static seq_t A351546_a(idx_t n) {
    if (n == 0) return 0;

    uint64_t primes[32];
    uint64_t exps[32];
    size_t pcnt;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    uint64_t nextp[32];
    for (size_t i = 0; i < pcnt; ++i)
        nextp[i] = next_prime(primes[i]);

    seq_t sigma = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t pw = 1;
        for (uint64_t k = 0; k < e + 1; ++k)
            pw *= (seq_t)p;
        seq_t term = (pw - 1) / (seq_t)(p - 1);
        sigma *= term;
    }

    uint64_t sig = (uint64_t)sigma;
    uint64_t sigpr[32];
    uint64_t sigex[32];
    size_t scnt;
    factor_u64(sig, sigpr, sigex, &scnt);

    seq_t result = 1;
    for (size_t i = 0; i < scnt; ++i) {
        uint64_t q = sigpr[i];
        bool exclude = false;
        for (size_t j = 0; j < pcnt; ++j) {
            if (q == nextp[j]) { exclude = true; break; }
        }
        if (!exclude) {
            seq_t mul = 1;
            for (uint64_t k = 0; k < sigex[i]; ++k)
                mul *= (seq_t)q;
            result *= mul;
        }
    }
    return result;
}

static seq_t A353666_gcd(seq_t x, seq_t y) {
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t v = A351546_a(n);
    return A353666_gcd((seq_t)n, v);
}
