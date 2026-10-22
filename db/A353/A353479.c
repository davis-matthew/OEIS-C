/*
 * A353479  a(n) = 1 if n is a semiprime of the form 4k+3, otherwise 0.
 * https://oeis.org/A353479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is a semiprime of the form 4k+3, otherwise 0
 * Range:     u8 n <= 9708, u16 n <= 9708, u32 n <= 9708, u64 n <= 9708,
 *            u128 n <= 9708, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Compute the A353479 sequence value for index n.
 * Returns 1 if n is a product of exactly two primes (semiprime)
 * and n % 4 == 3, else 0. */
seq_t a(idx_t n)
{
    /* Only candidates congruent to 3 modulo 4 can be in the sequence. */
    if ((n % 4U) != 3U)
        return 0;

    /* Factor n using the provided helper. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Count the total number of prime factors (with multiplicity). */
    uint64_t total = 0;
    for (size_t i = 0; i < cnt; ++i)
        total += exps[i];

    /* Return 1 iff exactly two prime factors are present. */
    return (seq_t)(total == 2);
}
