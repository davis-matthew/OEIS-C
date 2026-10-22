/*
 * A362296  Greatest common divisor of composite numbers between the n-th and
 *          (n+1)st primes.
 * https://oeis.org/A362296
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) + 1 if prime(n+1) = prime(n) + 2, otherwise 1.
 *            (offset 2, so a(2) corresponds to the first twin prime pair 3,5)
 * Range:     u8 n <= 56, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   // provides uint32_t get_prime(idx_t k)

/* Computes the nth term of OEIS sequence A362296 (offset 2). */
seq_t a(idx_t n)
{
    /* Below the offset: behavior unspecified */
    if (n < 2) {
        return (seq_t)0;
    }

    /* Retrieve the n-th and (n+1)-st primes. */
    uint32_t p_n   = get_prime(n);
    uint32_t p_n1  = get_prime(n + 1);

    /* Check for twin primes (difference of 2). */
    if (p_n1 - p_n == 2) {
        return (seq_t)(p_n + 1);
    } else {
        return (seq_t)1;
    }
}
