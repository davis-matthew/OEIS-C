/*
 * A359954  a(n) = Sum_{d|n} tau(d)*phi(d)*mu(n/d).
 * https://oeis.org/A359954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} tau(d)*phi(d)*mu(n/d)
 * Range:     u8 n <= 98, u16 n <= 9008, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (multiplicative: a(p) = 2p-3, a(p^e) = (e*p + p - e)*(p-1)*p^(e-2), e>1) */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* a(1) = 1 automatically handled (no prime factors) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        size_t e_int = (size_t)exps[i];

        seq_t term;
        if (e_int == 1) {
            term = (seq_t)2 * p - (seq_t)3;
        } else {
            seq_t part1 = (seq_t)e_int * p + p - (seq_t)e_int;
            seq_t part2 = p - (seq_t)1;

            /* compute p^(e-2) */
            seq_t part3 = 1;
            for (size_t k = 0; k < e_int - 2; ++k) {
                part3 *= p;
            }

            term = part1 * part2 * part3;
        }

        result *= term;
    }

    return result;
}
