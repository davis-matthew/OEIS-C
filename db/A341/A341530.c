/*
 * A341530  a(n) = gcd(n*sigma(A003961(n)), sigma(n)*A003961(n)), where
 *          A003961 shifts the prime factorization of n one step towards
 *          larger primes, and sigma is the sum of divisors of n.
 * https://oeis.org/A341530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n*sigma(A003961(n)), sigma(n)*A003961(n))
 * Range:     u8 n <= 7, u16 n <= 71, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   factor_u64, gcd, is_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

// Use the helper functions provided by the build
#include "../utils/gcd.h"
#include "../utils/sigma_of.h"
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 */
    seq_t N = (seq_t)n;                 /* convert to seq_t for arithmetic */

    /* Factorise n into its prime powers */
    size_t maxfactors = 64;             /* enough for all typical n */
    uint64_t primes[maxfactors];
    uint64_t exponents[maxfactors];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exponents, &cnt);

    /* Compute A003961(n): product of the next prime after every prime
       factor of n, each raised to the same exponent. */
    seq_t M = 1;                        /* will hold A003961(n) */
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        unsigned int e = (unsigned int)exponents[i];

        /* Find the next prime after p */
        uint64_t next_p = p + 1;
        while (!is_prime(next_p))
            ++next_p;

        /* Compute next_p^e as seq_t */
        seq_t pow_val = 1;
        for (unsigned int k = 0; k < e; ++k)
            pow_val *= next_p;

        /* Multiply into M */
        M *= pow_val;
    }

    /* sigma(n) and sigma(A003961(n)) */
    seq_t sigma_n = sigma_of((idx_t)n);
    seq_t sigma_m = sigma_of((idx_t)M);

    /* Compute the two arguments of the gcd */
    seq_t term1 = N * sigma_m;          /* n * sigma(A003961(n)) */
    seq_t term2 = sigma_n * M;          /* sigma(n) * A003961(n) */

    /* Return the gcd of the two terms */
    return gcd(term1, term2);
}
