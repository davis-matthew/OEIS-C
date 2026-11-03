/*
 * A362333  Least nonnegative integer k such that (gpf(n)!)^k is divisible by
 *          n, where gpf(n) is the greatest prime factor of n.
 * https://oeis.org/A362333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-01)
 * Helpers:   factor_u64, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the least nonnegative integer k such that (gpf(n)!)^k is
   divisible by n. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n) {
    if (n == 1)                 /* a(1) = 0 by definition */
        return 0;

    uint64_t nn = (uint64_t)n;  /* n fits in 64 bits for the factorisation routines */
    uint64_t g   = largest_prime_factor(nn);     /* greatest prime factor of n */

    /* Factorise n: primes[i]^exps[i] */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t maxk = 0;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* exponent of p in g! */
        uint64_t v   = 0;
        uint64_t pow = p;
        while (pow <= g) {
            v += g / pow;
            if (pow > g / p)     /* avoid overflow */
                break;
            pow *= p;
        }

        seq_t ek = (seq_t)e;
        seq_t ki = (ek + v - 1) / v;  /* ceil(e / v) */

        if (ki > maxk)
            maxk = ki;
    }

    return maxk;
}
