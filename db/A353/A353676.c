/*
 * A353676  a(n) = 1 if n is an odd number with exactly 2 distinct prime
 *          factors, otherwise 0.
 * https://oeis.org/A353676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is odd with exactly 2 distinct prime factors, else
 *            0
 * Range:     u8 n <= 9478, u16 n <= 9478, u32 n <= 9478, u64 n <= 9478,
 *            u128 n <= 9478, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n) {
    /* Even numbers cannot satisfy the condition */
    if ((n & 1) == 0) return (seq_t)0;

    /* Count the number of distinct prime factors of n */
    unsigned int d = count_distinct_primes((uint64_t)n);

    /* Return 1 if and only if exactly two distinct primes were found */
    return (seq_t)(d == 2);
}
