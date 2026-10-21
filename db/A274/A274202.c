/*
 * A274202  Primes congruent to 31 mod 65.
 * https://oeis.org/A274202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 140, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A274202: primes congruent to 31 modulo 65.
   Implementation: iterate over numbers of the form 31 + 65*k and test
   primality with the provided is_prime_small helper. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* Offset at 1: a(1) = 31 */
    if (n == 0) {   /* Undefined for n < 1, return 0 for safety */
        return (seq_t)0;
    }

    idx_t found = 0;          /* How many primes we have found */
    uint64_t candidate = 31;  /* First number in the residue class 31 mod 65 */

    while (1) {
        if (is_prime_small(candidate)) {
            found++;
            if (found == n) {
                return (seq_t)candidate;
            }
        }
        /* move to the next number congruent to 31 mod 65 */
        candidate += 65;
    }
}
