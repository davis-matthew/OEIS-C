/*
 * A372786  Number of divisors of 6n; a(n) = tau(6*n) = A000005(6*n).
 * https://oeis.org/A372786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(6*n)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 88 terms through n = 9769 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    /* compute m = 6 * n using the native idx_t type (uint64_t) */
    idx_t m = 6 * n;
    uint64_t m64 = m;          /* safe conversion to uint64_t */
    return (seq_t)num_divisors(m64);
}
