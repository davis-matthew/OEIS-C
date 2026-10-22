/*
 * A355452  a(n) = 1 if Bernoulli number B_{n} has denominator 6, otherwise 0.
 * https://oeis.org/A355452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the denominator of the Bernoulli number B_n is 6,
 *            otherwise 0. Equivalent condition: n is even and for every prime
 *            p ≥ 5, (p-1) does not divide n
 * Range:     u8 n <= 9779, u16 n <= 9779, u32 n <= 9779, u64 n <= 9779,
 *            u128 n <= 9779, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: a(1)=0, a(2)=1, a(14)=1, etc. */

#include <stdbool.h>

static bool is_prime_simple(idx_t x)
{
    if (x < 2) return false;
    if (x == 2 || x == 3) return true;
    if ((x & 1) == 0) return false;
    for (idx_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* Odd indices (including n=1) cannot have denominator 6. */
    if ((n & 1) != 0) return (seq_t)0;

    /* Start checking primes p >= 5 up to n+1. */
    for (idx_t p = 5; p <= n + 1; ++p)
    {
        if (!is_prime_simple(p))
            continue;
        /* p is a prime ≥ 5.  If (p-1) divides n, denominator would contain p. */
        if (n % (p - 1) == 0)
            return (seq_t)0;
    }

    /* Only primes 2 and 3 divide n, so denominator is 6. */
    return (seq_t)1;
}
