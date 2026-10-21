/*
 * A249734  Even bisection of A003961: Replace in 2n each prime factor p(k)
 *          with prime p(k+1).
 * https://oeis.org/A249734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the next prime for each prime factor of 2*n
 *            (i.e., a(n) = A003961(2*n) )
 * Range:     u8 n <= 23, u16 n <= 767, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Computes a(n) for the OEIS sequence A249734. */
seq_t a(idx_t n)
{
    /* For n=0 (outside of the official offset) we simply return 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Lazy static initialization of a prime list up to 30 000.
       That range comfortably covers 2*n (≤ 20 000) and the next prime
       after any factor ≤ 2*n. */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;
    static bool primes_built = false;

    if (!primes_built) {
        /* Build primes up to 30 000 and store them in primes. */
        build_primes_upto(30000, &primes, &prime_cnt);
        primes_built = true;
    }

    /* Compute m = 2*n and factor it. */
    idx_t m = 2 * n;
    seq_t result = (seq_t)1;

    /* Factorization by trial division using the precomputed primes. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p * p > m) {
            break;
        }
        if (m % p == 0) {
            /* Count exponent of prime p in m. */
            size_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            /* Next prime after p is primes[i+1] (safe because we built
               the sieve far enough). */
            uint64_t next_p = primes[i + 1];
            /* Multiply result by next_p^exp. */
            for (size_t e = 0; e < exp; ++e) {
                result *= (seq_t)next_p;
            }
        }
    }

    /* If after division m > 1, then m itself is a prime factor. */
    if (m > 1) {
        /* Locate m in the primes array. */
        size_t idx = 0;
        while (idx < prime_cnt && primes[idx] != m) {
            ++idx;
        }
        /* Next prime after m. */
        uint64_t next_p = primes[idx + 1];
        /* Exponent is 1 because m > 1 after removing smaller primes. */
        result *= (seq_t)next_p;
    }

    return result;
}
