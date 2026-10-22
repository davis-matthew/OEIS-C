/*
 * A392870  Multiplicative inverse of {n mod A117366(n)} in finite field
 *          F(A117366(n)), where A117366 gives the smallest prime greater than
 *          the largest prime dividing n.
 * https://oeis.org/A392870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n mod p)^{-1} in F(p), where p is the smallest prime
 *            greater than the largest prime factor of n
 * Range:     u8 n <= 292, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/mod_pow.h"

/* Return the n‑th term of sequence A392870. */
seq_t a(idx_t n)
{
    /* Work in 64‑bit arithmetic for the modulus. */
    uint64_t n64 = (uint64_t)n;

    /* Find the largest prime factor of n. */
    uint64_t maxPrime = 0;
    size_t cnt = 0;
    uint64_t primes[16], exps[16];
    if (n64 > 1) {
        factor_u64(n64, primes, exps, &cnt);
        for (size_t i = 0; i < cnt; ++i)
            if (primes[i] > maxPrime)
                maxPrime = primes[i];
    }

    /* For n = 1, set maxPrime to 1 so that next_prime(1) returns 2. */
    if (maxPrime == 0)
        maxPrime = 1;

    /* Next prime after the largest prime factor. */
    uint64_t p = next_prime(maxPrime);
    if (p <= 1)
        p = 2;

    uint64_t residue = n64 % p;        /* 1 ≤ residue < p for n ≥ 1 */

    /* Compute the multiplicative inverse of residue modulo p using Fermat's little theorem.
       Since p is prime, inverse = residue^{p-2} mod p. */
    seq_t inv = mod_pow((seq_t)residue, p - 2, (seq_t)p);
    return inv;
}
