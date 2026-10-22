/*
 * A353674  a(n) = 1 if n is an even number with an odd number of distinct
 *          prime factors, otherwise 0.
 * https://oeis.org/A353674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is even and has an odd number of distinct prime
 *            factors, otherwise 0
 * Range:     u8 n <= 9969, u16 n <= 9969, u32 n <= 9969, u64 n <= 9969,
 *            u128 n <= 9969, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* Only even n can be 1 */
    if ((n & 1) != 0)
        return (seq_t)0;

    /* Count distinct prime factors of n */
    int dpf = count_distinct_primes((uint64_t)n);

    /* Return 1 if the count is odd, else 0 */
    return (seq_t)(dpf & 1);
}
