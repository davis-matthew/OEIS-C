/*
 * A348992  a(n) = A000265(sigma(n)) / gcd(sigma(n), A003961(n)), where
 *          A003961(n) is fully multiplicative with a(prime(k)) = prime(k+1),
 *          and sigma is the sum of divisors function.
 * https://oeis.org/A348992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = odd_part(sigma(n)) / gcd(sigma(n), A003961(n))
 * Range:     u8 n <= 53, u16 n <= 8468, u32 n <= 8468, u64 n <= 8468,
 *            u128 n <= 8468, big any n
 * Helpers:   factor_u64, gcd, next_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/next_prime.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    /* Compute sigma(n) using the provided utility */
    seq_t sigma = sigma_of(n);

    /* Compute the odd part of sigma: divide by powers of 2 */
    seq_t odd_sigma = sigma;
    while ((odd_sigma & 1) == 0) {
        odd_sigma >>= 1;
    }

    /* Compute A003961(n):
       factor n, then product of nextprime(p)^e for each prime power p^e */
    uint64_t primes[64];
    uint64_t exponents[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exponents, &cnt);

    seq_t a003961 = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t nextp = next_prime(p);          /* next prime after p   */
        seq_t term = 1;
        for (uint64_t e = 0; e < exponents[i]; ++e) {
            term *= (seq_t)nextp;
        }
        a003961 *= term;
    }

    /* Compute gcd(sigma, A003961(n)) and divide the odd part of sigma by it */
    seq_t g = gcd(sigma, a003961);
    seq_t result = odd_sigma / g;   /* exact division under the sequence definition */

    return result;
}
