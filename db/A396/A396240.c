/*
 * A396240  Denominator of r(n) = (prime(n+2) - 2*prime(n+1) + prime(n)) /
 *          (prime(n+2) - prime(n)).
 * https://oeis.org/A396240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator of (prime(n+2) - 2*prime(n+1) + prime(n)) /
 *            (prime(n+2) - prime(n)) for n >= 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, u64_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"   // provides uint32_t get_prime(idx_t k)
#include "../utils/u64_gcd.h"     // provides uint64_t u64_gcd(uint64_t a, uint64_t b)

/* Compute the n-th term, with offset 1 (a(1) is the first term). */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. The behaviour for n == 0 is undefined. */
    if (n == 0) {
        return 0;
    }

    /* Retrieve the three consecutive primes needed. */
    uint32_t p1 = get_prime(n);       /* prime(n)   */
    uint32_t p2 = get_prime(n + 1);   /* prime(n+1) */
    uint32_t p3 = get_prime(n + 2);   /* prime(n+2) */

    /* Compute the absolute value of the numerator. */
    int64_t num_signed = (int64_t)p3 - 2LL * (int64_t)p2 + (int64_t)p1;
    uint64_t num = (num_signed >= 0) ? (uint64_t)num_signed : (uint64_t)-num_signed;

    /* Compute the denominator (always positive). */
    uint64_t denom = (uint64_t)p3 - (uint64_t)p1;

    /* Reduce the fraction and return the denominator. */
    uint64_t g = u64_gcd(num, denom);
    return (seq_t)(denom / g);
}
