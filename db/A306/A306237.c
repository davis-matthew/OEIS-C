/*
 * A306237  a(n) = primorial prime(n)#/prime(n - 1).
 * https://oeis.org/A306237
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = primorial(n) / prime(n-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Alternative representation: a(n) = prime(n) * primorial(n-2) */
/* Primorial of the nth prime: product of the first n primes. */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 2.  For smaller n we return 0. */
    if (n < 2)
        return 0;

    seq_t result = 1;

    /* Multiply all primes up to the (n-2)th prime. */
    if (n > 2) {
        for (idx_t i = 1; i <= n - 2; ++i) {
            result *= (seq_t)get_prime(i);
        }
    }

    /* Multiply by the nth prime. */
    result *= (seq_t)get_prime(n);

    return result;
}
