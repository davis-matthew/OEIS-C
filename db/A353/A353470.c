/*
 * A353470  a(n) = 1 if the number of its divisors, tau(n), is divisible by 3,
 *          otherwise 0.
 * https://oeis.org/A353470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the number of divisors tau(n) is divisible by 3,
 *            otherwise 0
 * Range:     u8 n <= 9289, u16 n <= 9289, u32 n <= 9289, u64 n <= 9289,
 *            u128 n <= 9289, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for n<1, harmless */
    uint64_t tau = divisor_count_u64((uint64_t)n);
    return (tau % 3 == 0) ? 1 : 0;
}
