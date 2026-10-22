/*
 * A368471  a(n) is the sum of exponentially odd divisors of the largest
 *          unitary divisor of n that is an exponentially odd number
 *          (A268335).
 * https://oeis.org/A368471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e‖n} ((p^(e+2) – p)/(p^2 – 1) + 1) if e is odd, 1
 *            otherwise. (A350389(n) is the largest unitary divisor of n that
 *            is exponentially odd.)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 4422, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;

    /* Factor n; at most 16 distinct primes for a 64‑bit number. */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* Even exponent → contribution 1. */
        if ((e & 1ULL) == 0)
            continue;

        /* Compute p^(e+2).  e is at most 63 for 2^63 < 2^64. */
        seq_t pow_pe2 = 1;
        for (uint64_t k = 0; k < e + 2; ++k)
            pow_pe2 *= (seq_t)p;

        seq_t numerator = pow_pe2 - (seq_t)p;
        seq_t denom     = (seq_t)p * (seq_t)p - 1;  /* p^2 – 1 */
        seq_t contrib   = numerator / denom + 1;   /* integer division */

        result *= contrib;
    }

    return result;
}
