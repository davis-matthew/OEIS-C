/*
 * A249224  Lpf (n(n+11)/2): least prime dividing n(n+11)/2.
 * https://oeis.org/A249224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least prime divisor of n*(n+11)/2
 * Range:     u8 n <= 502, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* compute m = n*(n+11)/2 using a type that cannot overflow */
    unsigned __int128 m_big = (unsigned __int128)n * ((unsigned __int128)n + 11) / 2;

    /* if m is even, 2 is the least prime factor */
    if ((m_big & 1) == 0)
        return (seq_t)2;

    /* trial division by successive primes until we find a divisor
       or exceed sqrt(m_big).  next_prime() gives the next prime > p. */
    uint64_t p = 3;
    for (;;) {
        unsigned __int128 p2 = (unsigned __int128)p * (unsigned __int128)p;
        if (p2 > m_big)            /* no divisor found */
            break;
        if (m_big % p == 0)        /* p divides m_big */
            return (seq_t)p;
        p = next_prime(p);
    }

    /* m_big itself is prime */
    return (seq_t)m_big;
}
