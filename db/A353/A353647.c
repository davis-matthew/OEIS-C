/*
 * A353647  Möbius transform of A062401, where A062401(n) = phi(sigma(n)).
 * https://oeis.org/A353647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} mu(n/d) * phi(sigma(d)) where mu is the Möbius
 *            function (A008683), sigma(d) is the sum of divisors of d, and
 *            phi is Euler's totient function
 * Range:     i8 n <= 59, i16 n <= 9496, i32 n <= 9496, i64 n <= 9496,
 *            i128 n <= 9496, big any n
 * Helpers:   factor_u64, mobius, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function enumerates all divisors d of n, computes
   mu(n/d) * phi(sigma(d)) for each divisor and accumulates
   the result.  Helper utilities are taken from the usual
   OEIS utility library. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/mobius.h"
#include "../utils/sigma_of.h"
#include "../utils/phi.h"

/* Recursively generate all divisors of n (given by its prime
   factorisation).  For each divisor d the accumulator is updated
   with mu(n/d) * phi(sigma(d)). */
static void
gen_divs (size_t idx, idx_t cur,
          const uint64_t *primes, const uint64_t *exps,
          size_t cnt, idx_t n, seq_t *acc)
{
    if (idx == cnt) {
        /* cur is a divisor d of n */
        seq_t sigma_d = sigma_of (cur);
        idx_t sigma_val = (idx_t)sigma_d;   /* convert to idx_t for phi */
        seq_t phi_sigma = phi (sigma_val);
        seq_t mu_val    = mobius (n / cur);
        *acc += mu_val * phi_sigma;
        return;
    }

    /* Enumerate powers of primes[idx] from exponent 0 up to exps[idx] */
    idx_t prod = cur;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        gen_divs (idx + 1, prod, primes, exps, cnt, n, acc);
        if (e < exps[idx])
            prod *= primes[idx];
    }
}

seq_t a(idx_t n)
{
    /* Factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64 (n, primes, exps, &cnt);

    seq_t result = 0;
    gen_divs (0, (idx_t)1, primes, exps, cnt, n, &result);
    return result;
}
