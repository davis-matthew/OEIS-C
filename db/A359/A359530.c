/*
 * A359530  Multiplicative with a(p^e) = (p + 4)^e.
 * https://oeis.org/A359530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product over prime powers p^e || n of (p + 4)^e
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 8191, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a is multiplicative with a(p^e) = (p + 4)^e */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t result = 1;          /* result multiplicative accumulator */
    idx_t temp = n;            /* work on a copy of n */

    /* trial division by all possible prime divisors */
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            /* count exponent of p in n */
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            /* multiply result by (p + 4)^exp */
            seq_t base = (seq_t)p + 4;
            for (idx_t i = 0; i < exp; ++i) {
                result *= base;
            }
        }
    }

    /* if a prime factor remains (either one prime or product of two primes > sqrt(temp)) */
    if (temp > 1) {
        seq_t base = (seq_t)temp + 4;
        result *= base;
    }

    return result;
}
