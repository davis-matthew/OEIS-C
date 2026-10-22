/*
 * A389087  Numerator of ratio sigma(n) / A048250(n), where sigma is the sum
 *          of divisors and A048250 is the sum of squarefree divisors.
 * https://oeis.org/A389087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A000203(n)/A389080(n)
 * Range:     u8 n <= 107, u16 n <= 8987, u32 n <= 8987, u64 n <= 8987,
 *            u128 n <= 8987, big any n
 * Helpers:   factor_u64, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the numerator of the reduced fraction σ(n)/A048250(n)
   where σ(n) is the sum of divisors and A048250(n) is the
   sum of squarefree divisors of n. */

#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

/* seq_t and idx_t are defined by the build. */
seq_t a(idx_t n)
{
    /* Edge case: n ≥ 1 is guaranteed by the caller. */
    uint64_t u = (uint64_t)n;          /* cast to uint64_t for factorization */

    /* Factor n: primes[0..cnt-1] are the distinct primes dividing n. */
    uint64_t primes[64];
    uint64_t exps[64];                 /* exponents are not needed here */
    size_t cnt = 0;
    factor_u64(u, primes, exps, &cnt);

    /* Compute sum of squarefree divisors: product over (1 + p). */
    seq_t sum_sqfree = 1;
    for (size_t i = 0; i < cnt; ++i) {
        sum_sqfree *= (seq_t)(primes[i] + 1);
    }

    /* Compute σ(n). */
    seq_t sigma = sigma_of(n);

    /* Reduce the fraction σ(n)/sum_sqfree.  The numerator is
       sigma divided by the greatest common divisor of sigma and
       sum_sqfree. */
    seq_t g = gcd(sigma, sum_sqfree);
    seq_t numerator = sigma / g;

    return numerator;
}
