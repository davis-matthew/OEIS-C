/*
 * A252489  Index of the largest prime which divides n*(n+1).
 * https://oeis.org/A252489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = π( max( largest_prime_divisor(n),
 *            largest_prime_divisor(n+1) ) ) where π(x) is the prime counting
 *            function. This follows from the definition that the largest
 *            prime divisor of n*(n+1) is the larger of the largest prime
 *            divisors of n and n+1, since n and n+1 are coprime
 * Range:     u8 n <= 1617, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 93 terms through n = 9665 (2026-11-01)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/prime_pi.h"

/* Return the largest prime divisor of m (m >= 2). */
static uint64_t largest_prime_divisor(uint64_t m)
{
    uint64_t largest = 0;
    /* factor 2 */
    while ((m & 1ULL) == 0) {
        largest = 2;
        m >>= 1;
    }
    /* odd factors */
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            largest = f;
            while (m % f == 0)
                m /= f;
        }
    }
    if (m > 1)            /* remaining m is prime */
        largest = m;
    return largest;
}

seq_t a(idx_t n)
{
    if (n == 0) /* undefined, but guard against 0 */
        return 0;

    /* The largest prime divisor of 1 is defined as 1 for the convenience
       of this function; the maximum with the divisor of 2 gives the
       correct result for a(1). */
    uint64_t lpf1 = (n == 1) ? 1 : largest_prime_divisor((uint64_t)n);
    uint64_t lpf2 = largest_prime_divisor((uint64_t)(n + 1));

    /* Determine the larger of the two largest prime divisors. */
    uint64_t largest = lpf1 > lpf2 ? lpf1 : lpf2;

    /* Compute π(largest) – the number of primes ≤ largest. */
    return prime_pi((idx_t)largest);
}
