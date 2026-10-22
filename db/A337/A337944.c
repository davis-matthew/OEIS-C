/*
 * A337944  Total number of divisors of the sum of the squared parts in each
 *          partition of n into two parts.
 * https://oeis.org/A337944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor(n/2)} tau(k^2 + (n-k)^2), where tau is
 *            the number of divisors
 * Range:     u8 n <= 41, u16 n <= 3527, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t half = n / 2;
    for (idx_t k = 1; k <= half; ++k)
    {
        uint64_t k64 = (uint64_t)k;
        uint64_t nk64 = (uint64_t)(n - k);
        uint64_t s = k64 * k64 + nk64 * nk64;
        uint64_t d = divisor_count_u64(s);
        sum += (seq_t)d;
    }
    return sum;
}
