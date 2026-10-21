/*
 * A275415  Pairs of primes (p, q) such that |2p - 3q| = 1.
 * https://oeis.org/A275415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A275415: list of primes p,q that satisfy |2p-3q|=1.
   The pairs are ordered by increasing p, and the sequence
   outputs p followed by its corresponding q.
   a(1)=5, a(2)=3, a(3)=7, a(4)=5, … */

#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* Count the terms already produced.  idx starts at 0. */
    idx_t idx = 0;

    /* The first prime to examine is already 3, so that on the first
       next_prime call we get 5, the first p in the sequence. */
    uint64_t p = 3;

    for (;;)
    {
        /* Get the next prime p.  next_prime returns the smallest
           prime strictly larger than its argument. */
        p = next_prime(p);

        /* For p ≡ 1 (mod 3) the equation 2p-3q = -1 gives
           an integer q; for p ≡ 2 (mod 3) the equation 2p-3q = +1
           does.  Either way, (2p±1)/3 is the unique candidate. */
        uint64_t q = (p % 3 == 1) ? (2 * p + 1) / 3
                                  : (2 * p - 1) / 3;

        /* If that candidate is prime we have a valid pair (p,q). */
        if (is_prime(q))
        {
            /* First term of the pair: p. */
            ++idx;
            if (idx == n) return (seq_t)p;

            /* Second term of the pair: q. */
            ++idx;
            if (idx == n) return (seq_t)q;
        }
        /* otherwise skip to the next prime. */
    }
}
