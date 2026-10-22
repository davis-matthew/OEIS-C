/*
 * A350812  a(n) = ceiling((n-R(ceiling(n^(1/2))))^2/(n+R(ceiling(n^(1/2))))),
 *          where R(ceiling(n^(1/2))) is the digit reversal of
 *          ceiling(n^(1/2)).
 * https://oeis.org/A350812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ceiling((n - R(ceil(sqrt(n))))^2 / (n + R(ceil(sqrt(n))))
 * Range:     u8 n <= 20, u16 n <= 544, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isqrt_floor_u64, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where R(x) is the decimal digit reversal.  */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/rev.h"   /* provides idx_t rev(idx_t x) */
#include "../utils/isqrt_floor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for non‑positive inputs */

    /* compute ceil(sqrt(n)) */
    idx_t floor_sqrt = isqrt_floor_u64(n);
    idx_t ceil_sqrt = floor_sqrt;
    if ((__uint128_t)ceil_sqrt * ceil_sqrt < n)
        ++ceil_sqrt;

    /* reversed digits of ceil_sqrt */
    seq_t r = rev(ceil_sqrt);

    /* difference and squared difference (in seq_t) */
    seq_t diff = (seq_t)(n - r);
    seq_t num   = diff * diff;

    /* denominator */
    seq_t den   = (seq_t)(n + r);

    /* ceiling of division */
    seq_t result = (num + den - 1) / den;
    return result;
}
