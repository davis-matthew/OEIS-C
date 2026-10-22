/*
 * A349162  a(n) = sigma(n) / gcd(sigma(n), A003961(n)), where A003961 shifts
 *          the prime factorization of n one step towards larger primes, and
 *          sigma is the sum of divisors function.
 * https://oeis.org/A349162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = sigma(n) / gcd(sigma(n), A003961(n)) where A003961(n) =
 *            ∏_{p^e∥n} next_prime(p)^e (next prime after each prime factor)
 * Range:     u8 n <= 53, u16 n <= 9932, u32 n <= 9932, u64 n <= 9932,
 *            u128 n <= 9932, big any n
 * Helpers:   factor_u64, gcd, next_prime, seq_pow, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/sigma_of.h"
#include "../utils/next_prime.h"
#include "../utils/gcd.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* n >= 1 by specification */
    if (n == 1) { return (seq_t)1; }

    /* Factorise n: primes[], exps[] hold its distinct prime factors and powers */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    /* Compute sigma(n) */
    seq_t sigma = sigma_of(n);

    /* Compute A003961(n) */
    seq_t a3 = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t q = next_prime(p);          /* next prime after p */
        seq_t term = seq_pow((seq_t)q, (idx_t)exps[i]);
        a3 = a3 * term;
    }

    /* Compute gcd and return sigma / gcd */
    seq_t g = gcd(sigma, a3);
    return sigma / g;
}
