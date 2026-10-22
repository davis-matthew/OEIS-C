/*
 * A379482  a(n) = sigma(A003961(n^2)), where A003961 is fully multiplicative
 *          with a(prime(i)) = prime(i+1), and sigma is the sum of divisors
 *          function.
 * https://oeis.org/A379482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sigma(A003961(n^2)) where A003961 is fully multiplicative
 *            with A003961(p) = nextprime(p). The computation uses the
 *            standard multiplicative formula for sigma and avoids any
 *            hard‑coded lookup
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 1023, u64 n <= 8961,
 *            u128 n <= 8961, big any n
 * Helpers:   big_pow, factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/big_pow.h"

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety, though no call with n=0 */

    /* Factor n.  We need primes and exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  factor_cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &factor_cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < factor_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];          /* exponent of p in n */
        uint64_t q = next_prime(p);    /* next prime after p */

        /* exponent in n² is 2*e, divisor‑sum term uses 2*e+1 */
        seq_t exp       = (seq_t)(2 * e + 1);
        seq_t pow_term  = big_pow((seq_t)q, exp);
        seq_t numerator = pow_term - (seq_t)1;
        seq_t denom     = (seq_t)(q - 1);
        seq_t term      = numerator / denom;

        result *= term;
    }

    return result;
}
