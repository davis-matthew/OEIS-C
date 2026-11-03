/*
 * A392865  a(n) = n mod A117366(n), where A117366 gives the smallest prime
 *          greater than the largest prime dividing n.
 * https://oeis.org/A392865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n mod A117366(n) where A117366(n) =
 *            next_prime(largest_prime_divisor(n))
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 109 terms through n = 9148 (2026-11-01)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements a(n) = n mod (smallest prime > largest prime factor of n) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

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
    /* For n = 1, largest_prime_divisor(1) returns 1 and
       next_prime(1) correctly yields 2, so the generic code works. */
    uint64_t lp = largest_prime_divisor(n);   /* largest prime divisor of n */
    uint64_t p  = next_prime(lp);             /* smallest prime greater than lp */
    seq_t r = (seq_t)(n % p);                 /* n modulo p */
    return r;
}
