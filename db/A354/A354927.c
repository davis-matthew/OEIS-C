/*
 * A354927  a(n) = 1 if the product of divisors of n is n^2, otherwise 0.
 * https://oeis.org/A354927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = 1 or the number of divisors of n is 4, otherwise
 *            0
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 141 terms through n = 9391 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    uint64_t d = num_divisors((uint64_t)n);
    return d == 4 ? 1 : 0;
}
