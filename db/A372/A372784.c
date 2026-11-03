/*
 * A372784  a(n) = tau(4*n) = A000005(4*n).
 * https://oeis.org/A372784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(4*n)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Validated: partial, pass, 76 terms through n = 76 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    /* Compute the number of divisors of 4*n. */
    uint64_t m = 4ull * (uint64_t)n;
    return (seq_t)num_divisors(m);
}
