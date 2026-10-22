/*
 * A392497  a(n) is the smallest p-rigid number where p = prime(n).
 * https://oeis.org/A392497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   get_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = smallest p-rigid number where p = prime(n).

   Definition of p-rigid (A392499):
     If p and p+2 are both prime (i.e. p is the lower member of a twin prime pair)
        a(p) = p^2 * (p+2).
     Otherwise
        a(p) = p · nextPrime(p) · nextPrime(nextPrime(p)).

   Special case for p = 2: a(1) = 20.  (2 has no twin on the right, but the
   sequence defines it as 20.)  */
 
#include "../utils/get_prime.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* Handle the unique first term. */
    if (n == 1)
        return (seq_t)20;

    /* Get the nth prime (p). */
    uint32_t p_uint = get_prime(n);
    seq_t p = (seq_t)p_uint;

    /* Find the next prime after p. */
    uint64_t next_p_uint = next_prime(p_uint);
    seq_t next_p = (seq_t)next_p_uint;

    /* Check whether p and p+2 are twin primes. */
    if (next_p_uint == (uint64_t)p_uint + 2ULL)
    {
        /* p is the lower member of a twin prime pair. */
        seq_t p_sq = p * p;
        return p_sq * next_p;          /* p^2 * (p+2) */
    }
    else
    {
        /* Find the prime after next_p. */
        uint64_t next_next_uint = next_prime(next_p_uint);
        seq_t next_next = (seq_t)next_next_uint;

        /* Return p * next_p * next_next. */
        return p * next_p * next_next;
    }
}
