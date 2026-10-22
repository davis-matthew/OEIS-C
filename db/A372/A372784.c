/*
 * A372784  a(n) = tau(4*n) = A000005(4*n).
 * https://oeis.org/A372784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(4*n)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* Compute the number of divisors of 4*n. */
    uint64_t m = 4ull * (uint64_t)n;
    return (seq_t)divisor_count_u64(m);
}
