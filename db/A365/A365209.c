/*
 * A365209  The sum of divisors d of n such that gcd(d, n/d) is a 3-smooth
 *          number (A003586).
 * https://oeis.org/A365209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (p^(e+1)-1)/(p-1) if p = 2 or 3, otherwise (1 +
 *            p^e) (product over prime power factors)
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* For the offset 1, a(1) = 1. Returning 0 for n = 0 is harmless. */
    if (n == 0) return 0;

    /* Max distinct prime factors of a 64‑bit integer is 20. */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t term;

        if (p == 2 || p == 3) {
            /* Compute (p^(e+1) - 1) / (p - 1) */
            seq_t pow = 1;
            for (uint64_t j = 0; j < e + 1; ++j) {
                pow *= (seq_t)p;
            }
            term = (pow - 1) / ((seq_t)p - 1);
        } else {
            /* Compute 1 + p^e */
            seq_t pow = 1;
            for (uint64_t j = 0; j < e; ++j) {
                pow *= (seq_t)p;
            }
            term = 1 + pow;
        }

        result *= term;
    }

    return result;
}
