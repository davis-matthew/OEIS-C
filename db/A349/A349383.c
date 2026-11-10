/*
 * A349383  a(n) = A349381(n) + A349382(n).
 * https://oeis.org/A349383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A349381(n) + A349382(n); both are multiplicative,
 *            computed from the factorization of n. A349381(p^e) =
 *            (nextprime(p) - prevprime(p)) * nextprime(p)^(e-1), with
 *            prevprime(2) = 1. A349382(2^e) = -2, and for p > 2, A349382(p^e)
 *            = prevprime(p)^(e-1) * (prevprime(p) - nextprime(p))
 * Range:     u8 n <= 63, u16 n <= 2047, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 101 terms through n = 8971 (2026-11-10)
 * Helpers:   factor_u64, is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

static idx_t A349383_prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t r1 = 1;   /* A349381 */
    seq_t r2 = 1;   /* A349382 */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t q = (seq_t)next_prime(p);

        /* A349381 part */
        seq_t r = (p == 2) ? (seq_t)1 : (seq_t)A349383_prev_prime((idx_t)p);
        seq_t qpow = 1;
        for (uint64_t k = 1; k < e; ++k) qpow *= q;
        r1 *= (q - r) * qpow;

        /* A349382 part */
        if (p == 2) {
            r2 *= (seq_t)0 - (seq_t)2;
        } else {
            seq_t pp = r;
            seq_t ppow = 1;
            for (uint64_t k = 1; k < e; ++k) ppow *= pp;
            r2 *= (pp * ppow) - q * ppow;
        }
    }

    return r1 + r2;
}
