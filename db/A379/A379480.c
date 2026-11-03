/*
 * A379480  a(n) = 1 if n is twice a square, otherwise 0.
 * https://oeis.org/A379480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is twice a square, otherwise 0
 * Range:     u8 n <= 100352, u16 n <= 100351, u32 n <= 100352,
 *            u64 n <= 100352, u128 n <= 100352, big any n
 * Validated: standard, pass, 145 terms through n = 9818 (2026-11-01)
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    /* If n is odd, it cannot be twice a square. */
    if (n & 1) return 0;

    /* Compute n/2. */
    seq_t half = ((seq_t)n) >> 1;

    /* Find the floor of the square root of half. */
    seq_t r = isqrt_seq(half);

    /* If r^2 == half, a(n) = 1, else 0. */
    return (r * r == half) ? 1 : 0;
}
