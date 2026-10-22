/*
 * A340373  a(n) = 1 if n is of the form of 2^i * p^j, with p an odd prime,
 *          and i>=0, j>=1, otherwise 0.
 * https://oeis.org/A340373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the odd part of n has exactly one distinct prime
 *            divisor, 0 otherwise
 * Range:     u8 n <= 9878, u16 n <= 9878, u32 n <= 9878, u64 n <= 9878,
 *            u128 n <= 9878, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    if (n < (idx_t)1)           /* OEIS offset starts at 1 */
        return (seq_t)0;

    uint64_t m = (uint64_t)n;   /* work on the 64‑bit representation of n */

    /* strip all powers of two */
    while ((m & 1ULL) == 0)
        m >>= 1;

    /* if odd part is 1, no odd prime factor exists */
    if (m <= 1ULL)
        return (seq_t)0;

    /* count distinct odd prime factors of the odd part */
    int distinct = count_distinct_primes(m);

    return (seq_t)(distinct == 1 ? 1 : 0);
}
