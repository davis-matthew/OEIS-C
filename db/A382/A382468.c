/*
 * A382468  a(n) = (largest prime factor of n) minus (its remaining distinct
 *          prime factors).
 * https://oeis.org/A382468
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2 * largest_prime_factor(n) -
 *            sum_of_distinct_prime_factors(n)
 * Range:     i8 n <= 130, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t largest_prime_factor(uint64_t n)
{
    uint64_t temp = n;
    uint64_t largest = 0;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            largest = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) largest = temp;
    return largest;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* sequence starts at n = 2 */

    uint64_t nn   = (uint64_t)n;
    uint64_t temp = nn;
    seq_t sum = 0;
    uint64_t largest = 0;

    /* factorization: collect distinct primes and their sum */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            sum += (seq_t)p;
            largest = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) {           /* remaining prime factor */
        sum += (seq_t)temp;
        largest = temp;
    }

    /* compute the formula */
    return (seq_t)(2 * (seq_t)largest - sum);
}
