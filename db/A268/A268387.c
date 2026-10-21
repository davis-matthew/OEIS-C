/*
 * A268387  Bitwise-XOR of the exponents of primes in the prime factorization
 *          of n.
 * https://oeis.org/A268387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = XOR of the exponents in the prime factorization of n,
 *            with a(1) = 0. This implements the definition of A268387
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>   /* for size_t */

/* a(n) returns the bitwise XOR of all exponents in the prime
   factorization of n.  For n = 1 the value is 0. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t xor_val = 0;           /* result to be returned */
    idx_t m = n;                 /* working copy of n */

    /* Treat factor 2 separately. */
    if ((m & 1) == 0) {          /* n is even */
        idx_t exp = 0;
        while ((m & 1) == 0) {   /* while n is divisible by 2 */
            ++exp;
            m >>= 1;             /* divide by 2 */
        }
        xor_val ^= (seq_t)exp;
    }

    /* Now test odd divisors up to sqrt(m). */
    for (idx_t p = 3; ( (__uint128_t)p * p ) <= (__uint128_t)m; p += 2) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                ++exp;
                m /= p;
            }
            xor_val ^= (seq_t)exp;
        }
    }

    /* If m > 1, it's a prime factor with exponent 1. */
    if (m > 1)
        xor_val ^= (seq_t)1;

    return xor_val;
}
