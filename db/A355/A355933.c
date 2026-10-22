/*
 * A355933  a(n) = A003973(n) / gcd(sigma(n), A003973(n)), where A003973(n) =
 *          sigma(A003961(n)) and A003961 is fully multiplicative with a(p) =
 *          nextprime(p).
 * https://oeis.org/A355933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = sigma(A003961(n)) / gcd(sigma(n), sigma(A003961(n)))
 * Range:     u8 n <= 12, u16 n <= 1457, u32 n <= 9046, u64 n <= 9046,
 *            u128 n <= 9046, big any n
 * Helpers:   factor_u64, gcd, next_prime, sigma_nextprime_power, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A003961 is fully multiplicative with a(p) = nextprime(p).               */

#include <stdint.h>
#include <stddef.h>

#include "../utils/gcd.h"
#include "../utils/sigma_of.h"
#include "../utils/next_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/sigma_nextprime_power.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* factorization of n */
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   cnt   = 0;
    factor_u64(n, primes, exps, &cnt);

    /* compute σ(A003961(n)) = Π σ( nextprime(p_i) ^ e_i ) */
    seq_t sigmaA = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t q = next_prime(p);
        sigmaA *= sigma_nextprime_power(q, e);
    }

    /* σ(n) */
    seq_t sigmaN = sigma_of(n);

    /* a(n) */
    seq_t g = gcd(sigmaA, sigmaN);
    return sigmaA / g;
}
