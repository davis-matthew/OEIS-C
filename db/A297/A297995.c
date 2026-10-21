/*
 * A297995  Number of ways to write n as 4*u + v^2 + x^3 + y^4 + 2*z^8, where
 *          u is 0 or 1, v is a positive integer and x,y,z are nonnegative
 *          integers.
 * https://oeis.org/A297995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to n = 4*u + v^2 + x^3 + y^4 + 2*z^8
 *            with u∈{0,1}, v a positive integer, x,y,z non‑negative integers
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; irrelevant below */
    seq_t count = 0;

    for (idx_t u = 0; u < 2; ++u) {
        seq_t base_u = (seq_t)4 * (seq_t)u;
        if (base_u > n) continue;
        seq_t rem_u = n - base_u;

        /* iterate over z : 2*z^8 <= rem_u */
        for (idx_t z = 0; ; ++z) {
            /* compute z^8 */
            seq_t zpow = 1;
            for (int i = 0; i < 8; ++i) zpow *= z;      /* zpow = z^8 */
            seq_t two_z = 2 * zpow;
            if (two_z > rem_u) break;
            seq_t rem_z = rem_u - two_z;

            /* iterate over y : y^4 <= rem_z */
            for (idx_t y = 0; ; ++y) {
                seq_t ypow = 1;
                for (int i = 0; i < 4; ++i) ypow *= y;   /* ypow = y^4 */
                if (ypow > rem_z) break;
                seq_t rem_y = rem_z - ypow;

                /* iterate over x : x^3 <= rem_y */
                for (idx_t x = 0; ; ++x) {
                    seq_t xpow = 1;
                    for (int i = 0; i < 3; ++i) xpow *= x;  /* xpow = x^3 */
                    if (xpow > rem_y) break;
                    seq_t rem_x = rem_y - xpow;

                    /* remaining part must be a positive square (v^2) */
                    if (rem_x == 0) continue;          /* v must be ≥1 */
                    seq_t s = floor_sqrt(rem_x);
                    if (s * s == rem_x) ++count;
                }
            }
        }
    }

    return count;
}
