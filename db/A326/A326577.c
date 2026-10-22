/*
 * A326577  a(n) = (2*n - 1) / A326478(2*n - 1).
 * https://oeis.org/A326577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A326577
 * Formula: a(n) = gcd(2*n-1, denominator of B_{2*(n-1)}),
 *   where the denominator of B_{2m} is the product of all primes p
 *   such that p-1 divides 2m.
 * In other words, a(n) = product of primes p | (2*n-1)
 *   for which (p-1) | (2*n-2).
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the offset: a(1) = 1 by definition (m = 1 has no prime factors) */
    if (n <= 1) {
        return (seq_t)1;
    }

    /* m = 2*n - 1 (odd), k = 2*n - 2 = 2*(n-1) */
    idx_t m = 2 * n - 1;
    idx_t k = 2 * n - 2;

    seq_t result = (seq_t)1;
    idx_t temp = m;

    /* factor m into distinct primes and test each for the condition */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            /* p is a prime factor of m */
            while (temp % p == 0) {
                temp /= p;
            }
            /* check the condition (p-1) | k */
            if ((k % (p - 1)) == 0) {
                result *= (seq_t)p;
            }
        }
    }
    /* if remaining temp is >1, it is a prime factor itself */
    if (temp > 1) {
        /* temp is prime, p = temp */
        if ((k % (temp - 1)) == 0) {
            result *= (seq_t)temp;
        }
    }

    return result;
}
