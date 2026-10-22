/*
 * A384531  Multiplicative sequence a(n) with a(p^e) = ((2*e+1) * p - 2*e) *
 *          p^(e-1) for prime p and e >= 0.
 * https://oeis.org/A384531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product_{p^e||n} ((2*e+1)*p - 2*e) * p^(e-1)
 * Range:     u8 n <= 29, u16 n <= 1259, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 1u) {
        return (seq_t)1;
    }

    /* factor_u64 writes the prime factors of n into these arrays.
       64 entries are more than enough for any 64‑bit n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];

        /* compute ((2*e+1)*p - 2*e) */
        seq_t part = ((seq_t)(2u * (seq_t)e + 1u) * p) - ((seq_t)(2u * (seq_t)e));

        /* compute p^(e-1) (for e>=1) */
        seq_t pow = (seq_t)1;
        for (uint64_t j = 1; j < e; ++j) {
            pow *= p;
        }

        result *= part * pow;
    }

    return result;
}
