/*
 * A351456  a(n) = A003958(sigma(A003961(n))), where A003958 is multiplicative
 *          with a(p^e) = (p-1)^e, A003961 multiplicative with a(prime(k)^e) =
 *          prime(1+k)^e, and sigma is the sum of divisors function.
 * https://oeis.org/A351456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = A003958(sigma(A003961(n)))
 * Range:     u8 n <= 12, u16 n <= 920, u32 n <= 9528, u64 n <= 9528,
 *            u128 n <= 9528, big any n
 * Helpers:   factor_u64, next_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Utilities used */
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Factor n and compute A003961(n) by replacing each prime factor
       with the next prime.  The result is stored in m.       */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t m = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p      = primes[i];
        uint64_t e      = exps[i];
        uint64_t nextp  = next_prime(p);                  /* next prime after p   */
        seq_t term = pow_uint(nextp, e);                  /* nextp^e              */
        m *= term;
    }

    /* Compute sigma(m).  Factor m and use the multiplicative
       formula sigma(p^e) = (p^(e+1)-1)/(p-1).           */
    factor_u64((uint64_t)m, primes, exps, &cnt);
    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t r = primes[i];
        uint64_t f = exps[i];
        seq_t r_pow = pow_uint(r, f + 1);                 /* r^(f+1) */
        seq_t numerator = r_pow - 1;
        seq_t denom     = (seq_t)(r - 1);
        seq_t sigma_r   = numerator / denom;
        sigma *= sigma_r;
    }

    /* Apply A003958 to sigma(m): for each prime factor s^g give
       (s-1)^g.                                                     */
    factor_u64((uint64_t)sigma, primes, exps, &cnt);
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t s = primes[i];
        uint64_t g = exps[i];
        seq_t term = pow_uint(s - 1, g);
        result *= term;
    }

    return result;
}
