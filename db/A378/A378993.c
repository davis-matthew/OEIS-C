/*
 * A378993  a(n) = n - omega(n), where omega = A001221.
 * https://oeis.org/A378993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - omega(n), where omega = A001221 (number of distinct
 *            prime factors)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* Undefined for n < 1, but return 0 */

    int omega = count_distinct_primes((uint64_t)n);
    return (seq_t)n - (seq_t)omega;
}
