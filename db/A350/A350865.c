/*
 * A350865  Sum of the larger parts in the partitions of n into two prime
 *          parts.
 * https://oeis.org/A350865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p <= q, p+q=n, p prime, q prime} q
 * Range:     u8 n <= 63, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE: This implementation relies on the `is_prime` function provided
   by the utils library.  It sums the larger part of each unordered
   partition of n into two primes. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;

    seq_t sum = 0;
    idx_t half = n / 2;
    for (idx_t p = 2; p <= half; ++p)
    {
        if (!is_prime(p))
            continue;

        idx_t q = n - p;
        if (q >= p && is_prime(q))
        {
            sum += (seq_t)q;
        }
    }

    return sum;
}
