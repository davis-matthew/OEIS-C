/*
 * A282029  a(n) = n - pi(n/2).
 * https://oeis.org/A282029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - pi(n/2)
 * Range:     u8 n <= 289, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 8462 (2026-11-01)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    idx_t half = n / 2;
    seq_t pi_half = prime_pi(half);
    return (seq_t)n - pi_half;
}
