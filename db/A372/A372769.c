/*
 * A372769  Primes p such that 2^p == p (mod nextprime(p)).
 * https://oeis.org/A372769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Incremental search for primes p such that 2^p ≡ p (mod nextprime(p)) */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"
#include "../utils/next_prime.h"

/* Return the n-th term of A372769 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0; /* undefined for offset 1, but safe */
    }

    uint64_t p = 2;           /* current prime */
    idx_t    found = 0;       /* how many terms found so far */

    while (true) {
        /* Find next prime after p. */
        uint64_t q = next_prime(p);

        /* Compute 2^p (mod q). */
        seq_t pow = mod_pow((seq_t)2, p, (seq_t)q);

        /* Check the condition. */
        if (pow == (seq_t)p) {
            ++found;
            if (found == n) {
                return (seq_t)p;
            }
        }

        /* Advance to the next prime. */
        p = q;
    }
}
