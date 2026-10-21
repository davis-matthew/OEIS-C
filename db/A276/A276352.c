/*
 * A276352  a(n) = 100^n - 10^n.
 * https://oeis.org/A276352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100^n - 10^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Compute 10^n and 100^n using the provided pow_uint utility,
       then subtract. Handles a(0) correctly because both powers are 1. */
    seq_t pow10_n   = pow_uint(10, n);
    seq_t pow100_n  = pow_uint(100, n);
    return pow100_n - pow10_n;
}
