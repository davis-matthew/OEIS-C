/*
 * A326619  Numerator of the average of the set of distinct prime indices of
 *          n.
 * https://oeis.org/A326619
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the average of the set of distinct prime
 *            indices of n. For n ≥ 2, factor n, compute the index of each
 *            distinct prime factor using prime_pi, sum the indices, reduce
 *            the sum/number_of_factors fraction, and return the reduced
 *            numerator
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   factor_u64, gcd, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;              /* sequence undefined below offset */

    /* arrays for prime factors and exponents; 64 is enough for 64‑bit n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(n, primes, exps, &cnt);

    /* sum of prime indices */
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        sum += prime_pi(primes[i]);   /* prime_pi returns seq_t */
    }

    seq_t denominator = (seq_t)cnt;
    seq_t g = gcd(sum, denominator);
    return sum / g;   /* reduced numerator */
}
