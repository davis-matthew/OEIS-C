/*
 * A253550  Shift one instance of the largest prime one step towards larger
 *          primes: a(1) = 1, for n>1: a(n) = (n / prime(g)) * prime(g+1),
 *          where g = A061395(n), index of the greatest prime dividing n.
 * https://oeis.org/A253550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n / p) * q, where p = largest prime divisor of n and q
 *            is the next prime after p. a(1) = 1
 * Range:     u8 n <= 161, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   largest_prime_divisor, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/largest_prime_divisor.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;                 /* a(1) = 1; a(0) is unused */

    uint64_t p = largest_prime_divisor((uint64_t)n);   /* largest prime divisor of n */
    uint64_t q = next_prime(p);                        /* next prime after p */

    seq_t n_seq = (seq_t)n;
    seq_t p_seq = (seq_t)p;
    seq_t q_seq = (seq_t)q;

    return (n_seq / p_seq) * q_seq;                /* (n/p) * q */
}
