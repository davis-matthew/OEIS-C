/*
 * A278114  Number of primes <= 2n^2.
 * https://oeis.org/A278114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of primes <= 2*n^2
 * Range:     u8 n <= 28, u16 n <= 640, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 74 terms through n = 4816 (2026-11-01)
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so only n >= 1 is relevant. */
    if (n < 1) return 0;

    /* Compute the upper bound 2*n^2 using idx_t to avoid overflow. */
    idx_t limit = (idx_t)2 * n * n;

    /* Return the prime‑counting function π(limit). */
    return prime_pi(limit);
}
