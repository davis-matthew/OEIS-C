/*
 * A348505  a(n) = usigma(n) / gcd(sigma(n), usigma(n)), where sigma is the
 *          sum of divisors function, A000203, and usigma is the unitary
 *          sigma, A034448.
 * https://oeis.org/A348505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = usigma(n) / gcd(sigma(n), usigma(n))
 * Range:     u8 n <= 255, u16 n <= 9974, u32 n <= 9974, u64 n <= 9974,
 *            u128 n <= 9974, big any n
 * Helpers:   factor_u64, gcd, pow_uint, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* usigma is the unitary sigma, sigma is sum of divisors. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/sigma_of.h"
#include "../utils/pow_uint.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Compute sigma(n) */
    seq_t sigma = sigma_of(n);

    /* Factorize n: primes[i] ^ exps[i]  for i = 0 .. cnt-1 */
    size_t cnt = 0;
    const size_t MAX_FACT = 64;         /* enough for n up to 10^12 and beyond */
    uint64_t primes[MAX_FACT];
    uint64_t exps[MAX_FACT];
    factor_u64(n, primes, exps, &cnt);

    /* Compute the unitary sigma: product over distinct primes of (1 + p^a) */
    seq_t usigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t pe   = pow_uint(primes[i], (idx_t)exps[i]);   /* p^a */
        usigma     = usigma * (pe + (seq_t)1);
    }

    /* Divide by gcd(sigma, usigma) */
    seq_t g = gcd(usigma, sigma);
    return usigma / g;
}
