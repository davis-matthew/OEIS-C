/*
 * A282463  Number of ways to write n as x^2 + y^2 + z^2 + w^2 with x,y,z,w
 *          nonnegative integers, x == y (mod 2) and z <= w such that both x
 *          and x^2 + 62*x*y + y^2 are squares.
 * https://oeis.org/A282463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 164, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of ways to write n as x^2 + y^2 + z^2 + w^2
 * with x,y,z,w nonnegative integers,
 * x == y (mod 2), z <= w,
 * x is a perfect square,
 * and x^2 + 62*x*y + y^2 is a perfect square.
 *
 * The implementation enumerates all admissible
 * values of x (restricted to perfect squares) and y,
 * then counts all unordered pairs (z,w) with z^2 + w^2 equal
 * to the remaining part of n.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t result = 0;

    /* iterate over a such that x = a^2 <= sqrt(n)   (x is a perfect square) */
    for (idx_t a = 0; ; ++a) {
        seq_t x = (seq_t)a * (seq_t)a;      /* x = a^2   */
        seq_t x2 = x * x;                   /* x^2 = a^4 */

        if (x2 > (seq_t)n)                  /* no larger x will work */
            break;

        /* remaining part that can be filled by y^2 (and maybe others) */
        seq_t rem_xy = (seq_t)n - x2;
        idx_t max_y = (idx_t)floor_sqrt(rem_xy);

        for (idx_t y = 0; y <= max_y; ++y) {
            /* parity: x == y (mod 2) */
            if (((idx_t)x & 1) != (y & 1))
                continue;

            /* check whether x^2 + 62*x*y + y^2 is a square */
            seq_t ysq = (seq_t)y * (seq_t)y;
            seq_t t   = x2 + (seq_t)62 * x * (seq_t)y + ysq;
            seq_t sqrt_t = floor_sqrt(t);
            if (sqrt_t * sqrt_t != t)
                continue;

            /* remaining part for z^2 + w^2 */
            seq_t rem_seq = (seq_t)n - x2 - ysq;
            if (rem_seq > (seq_t)n)          /* safety check */
                continue;

            /* count unordered pairs (z,w) with z <= w */
            seq_t half_rem = rem_seq / 2;           /* floor division */
            idx_t max_z = (idx_t)floor_sqrt(half_rem);

            for (idx_t z = 0; z <= max_z; ++z) {
                seq_t zsq = (seq_t)z * (seq_t)z;
                seq_t wsq = rem_seq - zsq;
                /* w >= z is guaranteed by z <= sqrt(rem_seq/2)   */
                seq_t w = floor_sqrt(wsq);
                if (w * w == wsq)
                    result += 1;
            }
        }
    }

    return result;
}
