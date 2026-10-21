/*
 * A264025  Number of ways to write n as x^2 + y*(2*y+1) + z*(z+1)/2 where x,
 *          y and z are nonnegative integers with z or z+1 prime.
 * https://oeis.org/A264025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as * x^2 + y*(2*y+1) +
 *            z*(z+1)/2, * with x,y,z ≥ 0 and z or z+1 prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code enumerates all feasible (z,y) pairs and checks whether the
 * remaining value is a perfect square. */

#include <stdbool.h>                    /* bool */
#include "../utils/is_prime.h"          /* is_prime(idx_t) */
#include "../utils/_floor_isqrt_u64.h"  /* _floor_isqrt_u64(idx_t) */

seq_t a(idx_t n)
{
    /* a(1) is defined; values for n < 1 are unused. */
    if (n == 0) return 0;

    idx_t count = 0;

    /* loop over z such that z*(z+1)/2 ≤ n */
    for (idx_t z = 1;; ++z) {
        idx_t z_tri = z * (z + 1) / 2;
        if (z_tri > n) break;

        /* only keep z if z or z+1 is prime */
        if (!is_prime(z) && !is_prime(z + 1)) continue;

        idx_t rem_z = n - z_tri;        /* value left for x^2 + y*(2*y+1) */

        /* loop over y such that y*(2*y+1) ≤ rem_z */
        for (idx_t y = 0;; ++y) {
            idx_t y_term = y * (2 * y + 1);
            if (y_term > rem_z) break;

            idx_t rem_xy = rem_z - y_term;

            /* check if rem_xy is a perfect square */
            idx_t s = _floor_isqrt_u64(rem_xy);
            if (s * s == rem_xy) ++count; /* found a valid (x,y) pair */
        }
    }

    return (seq_t)count;
}
