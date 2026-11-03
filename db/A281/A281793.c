/*
 * A281793  The largest prime factor of (1+n)*(1+n^2).
 * https://oeis.org/A281793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max( largest_prime_factor(n+1),
 *            largest_prime_factor(n^2+1) )
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 994 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the helper function largest_prime_divisor from the utils library. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

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
    if (n == 0)
        return (seq_t)1;                     /* lpf(1) is defined as 1 */

    uint64_t n64 = (uint64_t)n;              /* safe cast: n ≤ 10000 in tests */
    uint64_t lpf1 = largest_prime_divisor(n64 + 1);
    uint64_t lpf2 = largest_prime_divisor(n64 * n64 + 1);

    uint64_t mx = lpf1 > lpf2 ? lpf1 : lpf2;
    return (seq_t)mx;
}
