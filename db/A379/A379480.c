/*
 * A379480  a(n) = 1 if n is twice a square, otherwise 0.
 * https://oeis.org/A379480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is twice a square, otherwise 0
 * Range:     u8 n <= 127, u16 n <= 9818, u32 n <= 9818, u64 n <= 9818,
 *            u128 n <= 9818, big any n
 * Helpers:   sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/sqrt_floor.h"

seq_t a(idx_t n)
{
    /* If n is odd, it cannot be twice a square. */
    if (n & 1) return 0;

    /* Compute n/2. */
    seq_t half = ((seq_t)n) >> 1;

    /* Find the floor of the square root of half. */
    seq_t r = sqrt_floor(half);

    /* If r^2 == half, a(n) = 1, else 0. */
    return (r * r == half) ? 1 : 0;
}
