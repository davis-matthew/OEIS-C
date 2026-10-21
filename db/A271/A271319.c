/*
 * A271319  Number of distinct prime factors of the n-th n-gonal number
 *          (A060354).
 * https://oeis.org/A271319
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001221(A060354(n))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    uint64_t nn = (uint64_t)n;
    uint64_t val = nn + ((nn - 2) * nn * (nn - 1)) / 2;
    return (seq_t)count_distinct_primes(val);
}
