/*
 * A275299  Number of ordered ways to write n as x^3 + y^2 + z^2 + w^2 with x
 *          + y - z a square, where x,y,z,w are nonnegative integers with y <=
 *          z.
 * https://oeis.org/A275299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A275299: Number of ordered ways to write n as
 *  x^3 + y^2 + z^2 + w^2 with x + y - z a square,
 *  where x, y, z, w are nonnegative integers and y ≤ z.  */

/*  The algorithm enumerates all possible x, y, z,
 *  checks that the remaining value is a perfect square w^2,
 *  and that x + y - z is a perfect square.
 *  Complexity: O(⋯) ≈ 2·10^5 operations for n = 10 000.  */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/*  Test whether a non‑negative integer (idx_t) is a perfect square  */
static inline bool is_square_idx(idx_t v)
{
    seq_t s = floor_sqrt((seq_t)v);
    return s * s == (seq_t)v;
}

/*  Main sequence function  */
seq_t a(idx_t n)
{
    seq_t count = 0;

    for (idx_t x = 0; ; ++x) {
        seq_t x3 = (seq_t)x * x * x;
        if (x3 > (seq_t)n)              /* x^3 exceeds n  */
            break;

        seq_t rem_xy = (seq_t)n - x3;   /* remaining after x^3  */

        /*  y ≤ sqrt(rem_xy)  */
        idx_t y_max = (idx_t)floor_sqrt(rem_xy);
        for (idx_t y = 0; y <= y_max; ++y) {
            seq_t y2 = (seq_t)y * y;
            if (y2 > rem_xy)          /* skip impossible y  */
                break;

            seq_t rem_z = rem_xy - y2;   /* remaining after y^2  */

            /*  z ≥ y and z ≤ sqrt(rem_z)  */
            idx_t z_max = (idx_t)floor_sqrt(rem_z);
            for (idx_t z = y; z <= z_max; ++z) {
                seq_t z2 = (seq_t)z * z;
                if (z2 > rem_z)          /* skip impossible z  */
                    break;

                seq_t w2 = rem_z - z2;    /* remaining after z^2 */

                /*  w^2 must be a perfect square  */
                if (!is_square_idx((idx_t)w2))
                    continue;

                /*  x + y - z must be a non‑negative square  */
                long long deltaLL = (long long)x + (long long)y - (long long)z;
                if (deltaLL < 0)
                    continue;

                idx_t delta = (idx_t)deltaLL;
                if (!is_square_idx(delta))
                    continue;

                ++count;   /* Valid representation found  */
            }
        }
    }

    return count;
}
