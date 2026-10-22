/*
 * A344337  a(n) = 9^omega(n), where omega(n) is the number of distinct primes
 *          dividing n.
 * https://oeis.org/A344337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 9^ω(n), where ω(n) is the number of distinct prime
 *            factors of n. The helper function count_distinct_primes() is
 *            provided by the build environment
 * Range:     u8 n <= 29, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_distinct_primes.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Count distinct prime divisors of n. */
    int distinct = count_distinct_primes((uint64_t)n);

    /* Compute 9^distinct. */
    seq_t result = 1;
    for (int i = 0; i < distinct; ++i)
        result *= (seq_t)9;

    return result;
}
