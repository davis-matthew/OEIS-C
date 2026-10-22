/*
 * A374016  a(n) = 1 if n is a fourth power, else 0.
 * https://oeis.org/A374016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 1 if n is a perfect fourth power, otherwise 0
 * Range:     u8 n <= 9085, u16 n <= 9085, u32 n <= 9085, u64 n <= 9085,
 *            u128 n <= 9085, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses two successive integer square roots to obtain
   the integer fourth root of n. */
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* Compute floor(sqrt(sqrt(n))) which equals floor(n^(1/4)) */
    idx_t r = _floor_isqrt_u64(_floor_isqrt_u64(n));

    /* Compute r^4 safely: r^2 fits in 32 bits, r^4 fits in 64 bits. */
    idx_t sq = r * r;        /* r^2 */
    idx_t r4 = sq * sq;      /* r^4 */

    return (seq_t)(r4 == n);
}
