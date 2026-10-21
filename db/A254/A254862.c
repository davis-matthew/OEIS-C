/*
 * A254862  Least prime p such that p + nextprime(p) is a multiple of n.
 * https://oeis.org/A254862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p such that p + nextprime(p) is a multiple of
 *            n
 * Range:     u8 n <= 28, u16 n <= 1978, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined for n ≥ 1; a(1) = 2.  The following routine
 * searches primes in increasing order until it finds such a prime.
 * It uses the generic primality test `is_prime(idx_t x)` available
 * from "../utils/is_prime.h".  All intermediate arithmetic is done in
 * uint64_t, which is sufficient for n up to 10000, because all terms
 * that appear in the sequence for these n are comfortably less than
 * 2^63.  The result is then cast to seq_t, which is guaranteed to
 * accommodate the value. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥ 1 by the interface contract. */
    uint64_t p = 2;                 /* first prime */
    while (1) {
        /* find the next prime after p */
        uint64_t q = p + 1;
        while (!is_prime(q))
            ++q;

        /* check if (p + q) is a multiple of n */
        if (((p % n) + (q % n)) % n == 0)
            return (seq_t)p;

        /* continue with the next prime */
        p = q;
    }
}
