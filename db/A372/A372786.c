/*
 * A372786  Number of divisors of 6n; a(n) = tau(6*n) = A000005(6*n).
 * https://oeis.org/A372786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(6*n)
 * Range:     u8 n <= 9769, u16 n <= 9769, u32 n <= 9769, u64 n <= 9769,
 *            u128 n <= 9769, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* compute m = 6 * n using the native idx_t type (uint64_t) */
    idx_t m = 6 * n;
    uint64_t m64 = m;          /* safe conversion to uint64_t */
    return (seq_t)divisor_count_u64(m64);
}
