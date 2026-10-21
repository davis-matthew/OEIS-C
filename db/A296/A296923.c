/*
 * A296923  Primes p such that Legendre(-5,p) = -1.
 * https://oeis.org/A296923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 3302, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A296923: Primes p such that Legendre(-5,p) = -1
   Equivalently, returned primes are 2 or occupy residue classes
   11, 13, 17, 19 (mod 20). This function generates the sequence
   by trial‑ending the next prime that satisfies the congruence. */

#include <stdint.h>      /* uint64_t */
#include <stdbool.h>    /* bool */

#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

seq_t a(idx_t n)
{
    /* Handle the first term separately. */
    if (n == 1)
        return (seq_t)2;

    /* Count of found terms (including the initial prime 2). */
    idx_t  count = 1;          /* we already have the prime 2 */
    uint64_t candidate = 3;    /* start searching from the first odd prime */

    while (count < n) {
        /* Fast path: skip even numbers. */
        if (is_prime(candidate)) {
            uint64_t r = candidate % 20;
            if (r == 11 || r == 13 || r == 17 || r == 19) {
                ++count;
                if (count == n)
                    return (seq_t)candidate;
            }
        }
        candidate += 2;          /* next odd candidate */
    }

    /* For completeness (never reached). */
    return (seq_t)0;
}
