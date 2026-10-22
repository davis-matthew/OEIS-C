/*
 * A361467  a(n) = A003961(n) * sigma(A003961(n)), where A003961 is fully
 *          multiplicative with a(p) = nextprime(p), and sigma is the sum of
 *          divisors function.
 * https://oeis.org/A361467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = A003961(n) * sigma(A003961(n)) where A003961 is
 *            multiplicative with a(p) = nextPrime(p)
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 1023, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
#include "../utils/factor_u64.h"

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    /* a(1) = 1 by definition. */
    if (n == (idx_t)1) return (seq_t)1;

    /* Factorise n.  We can reserve a maximum of 64 prime factors
       which is sufficient for any 64‑bit integer. */
    uint64_t *primes = (uint64_t *)malloc(64 * sizeof(uint64_t));
    uint64_t *exps   = (uint64_t *)malloc(64 * sizeof(uint64_t));
    if (!primes || !exps) return (seq_t)0; /* out of memory */

    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t q = next_prime(p);

        /* q^e */
        seq_t qpow = (seq_t)1;
        for (uint64_t k = 0; k < e; ++k)
            qpow *= (seq_t)q;

        /* sigma(q^e) = 1 + q + ... + q^e */
        seq_t sigma = (seq_t)0;
        seq_t power = (seq_t)1;
        for (uint64_t t = 0; t <= e; ++t)
        {
            sigma += power;
            power *= (seq_t)q;
        }

        /* contribution from this prime factor */
        result *= qpow * sigma;
    }

    free(primes);
    free(exps);
    return result;
}
