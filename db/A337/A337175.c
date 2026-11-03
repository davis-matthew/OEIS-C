/*
 * A337175  Number of pairs of divisors of n, (d1,d2), such that d1 <= d2 and
 *          d1*d2 < n.
 * https://oeis.org/A337175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(tau(n)^2 / 4) where tau(n) is the number of
 *            divisors of n
 * Range:     u8 n <= 839, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9454 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* OEIS offset starts at 1 */

    /* divisor_count_u64 is provided in the utils set */
    uint64_t tau = num_divisors((uint64_t)n);

    /* compute floor(tau^2 / 4) – 64‑bit arithmetic is enough for all
       values that can appear in the tested range. */
    uint64_t prod = tau * tau;
    uint64_t res  = prod / 4;

    return (seq_t)res;                   /* cast to the build's seq_t type */
}
