/*
 * A372565  a(n) is the greatest common divisor of n, sigma(n) and A003961(n),
 *          where A003961(n) is fully multiplicative function with a(prime(i))
 *          = prime(i+1).
 * https://oeis.org/A372565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd(n, sigma(n), A003961(n))
 * Range:     u8 n <= 53, u16 n <= 9851, u32 n <= 9851, u64 n <= 9851,
 *            u128 n <= 9851, big any n
 * Helpers:   factor_u64, gcd, next_prime, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A003961(n) = ∏_{p^e||n} next_prime(p)^e      */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/sigma_of.h"
#include "../utils/gcd.h"
#include "../utils/next_prime.h"

/* Compute the sequence term a(n) for n ≥ 1.  The arguments and return
   type are defined by the build (idx_t = uint64_t, seq_t arbitrary
   precision).  No global state is required. */
seq_t a(idx_t n)
{
    /* Factor n into primes and exponents. */
    size_t cnt = 0;
    uint64_t primes[64];
    uint64_t exps[64];
    factor_u64(n, primes, exps, &cnt);

    /* Σ(n) */
    seq_t sigma = sigma_of(n);

    /* A003961(n) = ∏ next_prime(p)^e */
    seq_t a003961 = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t np  = next_prime(p);   /* next prime after p */
        uint64_t e   = exps[i];
        seq_t term = 1;
        while (e--) {
            term *= (seq_t)np;
        }
        a003961 *= term;
    }

    /* gcd(n, σ(n), A003961(n)) */
    seq_t g = gcd((seq_t)n, sigma);
    g = gcd(g, a003961);
    return g;
}
