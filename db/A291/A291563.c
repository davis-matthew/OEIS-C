/*
 * A291563  Number of partitions of 2n into two prime parts or two nonprime
 *          parts.
 * https://oeis.org/A291563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} [is_prime(i) == is_prime(2n-i)]
 * Range:     u8 n <= 311, u16 n <= 9447, u32 n <= 9447, u64 n <= 9447,
 *            u128 n <= 9447, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count partitions of 2n into two primes or two nonprimes. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* Provided helper to test primality */

/* External linkage as required by the build system. */
seq_t a(idx_t n)
{
    seq_t count = 0;                /* Result accumulator. */
    idx_t twon = 2 * n;             /* 2n fits in idx_t for the intended range. */

    for (idx_t i = 1; i <= n; ++i) {
        idx_t j = twon - i;        /* Complement to 2n. */

        /* The helper function is_prime returns false for 0 and 1. */
        bool pi = is_prime(i);
        bool pj = is_prime(j);

        if (pi == pj)             /* Both prime or both nonprime. */
            ++count;
    }

    return count;
}
