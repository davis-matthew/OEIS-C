/*
 * A350337  Sum of the divisors of n that are prime or the product of 2 (not
 *          necessarily distinct) primes.
 * https://oeis.org/A350337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Sum_{d|n} d * (u(d)+v(d)) where u(d) = 1 if d is prime
 *            else 0, and v(d) = 1 if d is a semiprime (prime squared or
 *            product of two primes) else 0
 * Range:     u8 n <= 177, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#include "../utils/is_prime_small.h"          /* bool is_prime_small(uint64_t n) */
#include "../utils/_smallest_prime_factor.h"  /* uint64_t _smallest_prime_factor(uint64_t n) */

static inline bool is_prime_or_semiprime(uint64_t x)
{
    if (x < 2) return false;
    if (is_prime_small(x)) return true;

    uint64_t p = _smallest_prime_factor(x);
    if (p == 0) return false;          /* safety, should not occur for composite x */

    uint64_t q = x / p;
    return is_prime_small(q);
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    seq_t sum = 0;

    for (uint64_t d = 1; d * d <= N; ++d)
    {
        if (N % d) continue;

        if (is_prime_or_semiprime(d))
            sum += (seq_t)d;

        uint64_t e = N / d;
        if (e != d && is_prime_or_semiprime(e))
            sum += (seq_t)e;
    }

    return sum;
}
