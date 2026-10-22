/*
 * A306250  Number of ways to write n as x*(3x+1) + y*(3y-1) + z*(3z+2) +
 *          w*(3w-2), where x,y,z,w are nonnegative integers with x*y*z = 0.
 * https://oeis.org/A306250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non‑negative integer quadruples (x,y,z,w)
 *            satisfying x*(3x+1) + y*(3y-1) + z*(3z+2) + w*(3w-2) = n with
 *            x*y*z = 0
 * Range:     u8 n <= 4, u16 n <= 1267, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Helper: returns true iff there exists w >= 0 such that w*(3w-2) = R */
static inline bool has_w_term(seq_t R) {
    if (R == 0)            /* w = 0 */
        return true;
    seq_t D = 4 + (seq_t)12 * R;      /* discriminant 4 + 12*R */
    seq_t s = floor_sqrt(D);
    if (s * s != D)        /* not a perfect square */
        return false;
    /* For R > 0, w is integer iff sqrt(D) % 6 == 4  (gives the + branch) */
    return (s % 6) == 4;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t sn = (seq_t)n;    /* promote n to seq_t for comparisons */

    /* Compute maximal bounds for x, y, z such that their individual terms
       do not exceed n.  All counts are very small for the given n ranges. */

    idx_t max_x = 0;
    for (;;) {
        seq_t term = (seq_t)max_x * (3*(seq_t)max_x + 1);
        if (term > sn) break;
        ++max_x;
    }
    if (max_x > 0) --max_x;   /* last valid value */

    idx_t max_y = 0;
    for (idx_t y = 1;; ++y) {
        seq_t term = (seq_t)y * (3*(seq_t)y - 1);
        if (term > sn) { max_y = y-1; break; }
    }

    idx_t max_z = 0;
    for (idx_t z = 1;; ++z) {
        seq_t term = (seq_t)z * (3*(seq_t)z + 2);
        if (term > sn) { max_z = z-1; break; }
    }

    /* Enumerate all (x,y,z) with at least one of them zero and total term <= n.
       For each, check whether the remaining part matches a valid w‑term. */
    for (idx_t x = 0; x <= max_x; ++x) {
        seq_t tx = (seq_t)x * (3*(seq_t)x + 1);
        if (tx > sn) break;                /* safety check */

        for (idx_t y = 0; y <= max_y; ++y) {
            seq_t ty = 0;
            if (y > 0) ty = (seq_t)y * (3*(seq_t)y - 1);
            seq_t txy = tx + ty;
            if (txy > sn) break;            /* increasing y ensures break */

            for (idx_t z = 0; z <= max_z; ++z) {
                if (x > 0 && y > 0 && z > 0) continue;   /* need x*y*z = 0 */

                seq_t tz = 0;
                if (z > 0) tz = (seq_t)z * (3*(seq_t)z + 2);
                seq_t t = txy + tz;
                if (t > sn) break;            /* increasing z ensures break */

                seq_t R = sn - t;
                if (has_w_term(R))
                    ++count;
            }
        }
    }
    return count;
}
