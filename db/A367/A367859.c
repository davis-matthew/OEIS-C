/*
 * A367859  Multiset multiplicity cokernel (MMC) of n. Product of (greatest
 *          prime factor with exponent k)^(number of prime factors with
 *          exponent k) over all distinct exponents k appearing in the prime
 *          factorization of n.
 * https://oeis.org/A367859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k} (max prime with exponent k)^{(number of primes with
 *            exponent k)}
 * Range:     u8 n <= 33, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the multiset multiplicity cokernel (MMC) of n by factorizing n, then for each
   distinct exponent k taking the largest prime that occurs with exponent k and raising
   that prime to the power equal to how many primes have exponent k. */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Arrays up to exponent 64 (max for 64‑bit n). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;

    /* Factor n.  factor_u64 returns arrays of primes and their exponents. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* For each exponent k store the largest prime and the number of primes with that exponent. */
    uint64_t maxPrime[65];
    size_t   primeCnt[65];
    for (size_t i = 0; i < 65; ++i) {
        maxPrime[i] = 0;
        primeCnt[i] = 0;
    }

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        uint64_t p = primes[i];
        if (primeCnt[e] == 0) {
            maxPrime[e] = p;
            primeCnt[e] = 1;
        } else {
            if (p > maxPrime[e]) maxPrime[e] = p;
            ++primeCnt[e];
        }
    }

    /* Accumulate result.  For each exponent e with non‑zero count, multiply by maxPrime[e]^primeCnt[e]. */
    seq_t result = (seq_t)1;
    for (uint64_t e = 1; e < 65; ++e) {
        if (primeCnt[e] > 0) {
            seq_t base = (seq_t)maxPrime[e];
            seq_t exp  = (seq_t)primeCnt[e];
            result = result * big_pow(base, exp);
        }
    }
    return result;
}
