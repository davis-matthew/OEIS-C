/*
 * A319178  a(n) is the number of decompositions of 2n into an unordered sum
 *          of two odd primes p and q such that at least one number among p-6
 *          and q-6 is also prime, and both p and q are greater than 3.
 * https://oeis.org/A319178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A319178:
   a(n) = number of unordered decompositions of 2n into two odd primes p,q > 3
   such that at least one of p-6 or q-6 is also prime. */

/* Use the provided primality test. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; for small n the answer is 0. */
    if (n < 1) {
        return (seq_t)0;
    }

    idx_t target = 2 * n;           /* 2n */
    seq_t count = 0;

    /* Iterate over odd primes p > 3 up to target/2 (unordered pairs). */
    for (idx_t p = 5; p <= target / 2; p += 2) {
        if (!is_prime(p)) {
            continue;               /* p must be prime */
        }
        idx_t q = target - p;       /* the complementary prime */
        if (q < p) {
            continue;               /* ensure unordered: p <= q */
        }
        if (q < 5) {                /* q must be > 3 and odd */
            continue;
        }
        if (!is_prime(q)) {
            continue;               /* q must be prime */
        }

        /* Check the condition involving p-6 and q-6. */
        bool good = false;
        if (p > 6 && is_prime(p - 6)) {
            good = true;
        }
        if (!good && q > 6 && is_prime(q - 6)) {
            good = true;
        }

        if (good) {
            ++count;
        }
    }

    return count;
}
