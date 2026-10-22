/*
 * A353673  a(n) = 1 if n is an odd number with an odd number of distinct
 *          prime factors, otherwise 0.
 * https://oeis.org/A353673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is odd and has an odd number of distinct prime
 *            factors, otherwise 0
 * Range:     u8 n <= 7688, u16 n <= 7688, u32 n <= 7688, u64 n <= 7688,
 *            u128 n <= 7688, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    if ((n & 1ULL) == 0)
        return (seq_t)0;
    int d = count_distinct_primes((uint64_t)n);
    return (seq_t)(d & 1);
}
