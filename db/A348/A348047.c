/*
 * A348047  a(n) = gcd(sigma(n), A003959(n)), where A003959 is multiplicative
 *          with a(p^e) = (p+1)^e and sigma is the sum of divisors function.
 * https://oeis.org/A348047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(σ(n), ∏(p+1)^e) where σ(n) is the sum of divisors and
 *            the product runs over the prime factorisation n = ∏ p^e
 * Range:     u8 n <= 71, u16 n <= 9452, u32 n <= 9452, u64 n <= 9452,
 *            u128 n <= 9452, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* compute σ(n) and ∏(p+1)^e */
    seq_t sigma = 1;
    seq_t prod   = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];

        /* σ contribution: 1 + p + p^2 + … + p^e */
        seq_t term_sigma = 0;
        seq_t term_pow   = 1;
        for (uint64_t k = 0; k <= e; ++k) {
            term_sigma += term_pow;
            term_pow   *= (seq_t)p;
        }
        sigma *= term_sigma;

        /* product contribution: (p+1)^e */
        seq_t base_pow = 1;
        seq_t base     = (seq_t)(p + 1);
        for (uint64_t k = 0; k < e; ++k) {
            base_pow *= base;
        }
        prod *= base_pow;
    }

    return gcd(sigma, prod);
}
