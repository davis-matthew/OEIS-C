/*
 * A354927  a(n) = 1 if the product of divisors of n is n^2, otherwise 0.
 * https://oeis.org/A354927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 1 or the number of divisors of n is 4, otherwise
 *            0
 * Range:     u8 n <= 9391, u16 n <= 9391, u32 n <= 9391, u64 n <= 9391,
 *            u128 n <= 9391, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    uint64_t d = divisor_count_u64((uint64_t)n);
    return d == 4 ? 1 : 0;
}
