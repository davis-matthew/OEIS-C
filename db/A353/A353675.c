/*
 * A353675  a(n) = 1 if n is an odd number with an even number of distinct
 *          prime factors, otherwise 0.
 * https://oeis.org/A353675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is odd and has an even number of distinct prime
 *            factors, otherwise 0
 * Range:     u8 n <= 9906, u16 n <= 9906, u32 n <= 9906, u64 n <= 9906,
 *            u128 n <= 9906, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* respect the OEIS offset: a(1) is defined, a(0) is not part of the sequence */
    if (n < 1) return 0;

    /* even numbers cannot satisfy the condition */
    if ((n & 1) == 0) return 0;

    /* count the distinct prime factors of n */
    int distinct = count_distinct_primes((uint64_t)n);

    /* return 1 if the count is even, else 0 */
    return (distinct % 2 == 0) ? 1 : 0;
}
