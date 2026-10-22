/*
 * A341526  Numerator of ratio n*sigma(A003961(n)) / sigma(n)*A003961(n),
 *          where sigma is the sum of divisors of n, and A003961 shifts the
 *          prime factorization of n one step towards larger primes.
 * https://oeis.org/A341526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 63, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   factor_u64, gcd, next_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A341526: numerator of the reduced fraction
 *  n * sigma(A003961(n))  /  sigma(n) * A003961(n)   */
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"
#include "../utils/pow_uint.h"

static seq_t sigma_of_factors(const uint64_t *primes, const uint64_t *exps,
                              size_t cnt, uint64_t offset)
{
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t e = (seq_t)exps[i];
        /* term = (p^{e+1} - 1) / (p - 1)  (or offset version) */
        seq_t num = pow_uint((uint64_t)p, (idx_t)(e + 1)) - 1;
        seq_t den = offset ? (seq_t)(offset + (i == 0 ? 0 : 0)) : (p - 1);
        /* For A003961, we multiply p by 1; here offset handles shifting outside */
        result *= num / den;
    }
    return result;
}

static seq_t a003961_value(const uint64_t *primes, const uint64_t *exps,
                           size_t cnt, seq_t *sigma_out)
{
    seq_t value  = 1;
    seq_t sigma  = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t nextp = next_prime(primes[i]);          /* shift to next prime */
        seq_t  pexp   = pow_uint(nextp, (idx_t)exps[i]);  /* nextp^e */
        value *= pexp;
        seq_t num = pow_uint(nextp, (idx_t)(exps[i] + 1)) - 1;
        seq_t den = nextp - 1;
        sigma *= num / den;
    }
    if (sigma_out) *sigma_out = sigma;
    return value;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Factor n using the utility function */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute sigma(n) */
    seq_t sigma_n = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p   = (seq_t)primes[i];
        seq_t e   = (seq_t)exps[i];
        seq_t num = pow_uint((uint64_t)p, (idx_t)(e + 1)) - 1;
        seq_t den = p - 1;
        sigma_n *= num / den;
    }

    /* Compute A003961(n) and its sigma */
    seq_t a001;          /* A003961(n) */
    seq_t sigma_a001;    /* sigma(A003961(n)) */
    a001 = a003961_value(primes, exps, cnt, &sigma_a001);

    /* Reduced numerator of n*sigma_a001 / (sigma_n*a001) */
    seq_t numerator   = (seq_t)n * sigma_a001;
    seq_t denominator = sigma_n * a001;
    seq_t g = gcd(numerator, denominator);
    return numerator / g;
}
