/*
 * A353470  a(n) = 1 if the number of its divisors, tau(n), is divisible by 3,
 *          otherwise 0.
 * https://oeis.org/A353470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the number of divisors tau(n) is divisible by 3,
 *            otherwise 0
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 137 terms through n = 9289 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for n<1, harmless */
    uint64_t tau = num_divisors((uint64_t)n);
    return (tau % 3 == 0) ? 1 : 0;
}
