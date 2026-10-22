/*
 * A318984  Theta series of quadratic form x^2 + x*y + 17*y^2.
 * https://oeis.org/A318984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of integer pairs (x,y) satisfying x^2 + x*y +
 *            17*y^2 = n
 * Range:     u8 n <= 15, u16 n <= 3036, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t total = 0;
    seq_t n_seq = (seq_t)n;

    /* Maximum |y| that can appear in any solution:
       n >= 17*y^2 - y^2/4 = 67*y^2/4   =>   |y| <= sqrt(4*n/67)  */
    seq_t max_y2 = (seq_t)4 * n_seq / 67;
    seq_t ymax = floor_sqrt(max_y2);

    for (seq_t y = 0; y <= ymax; ++y) {
        seq_t y2 = y * y;
        seq_t delta = (seq_t)4 * n_seq - (seq_t)67 * y2;
        if (delta < 0) continue;

        seq_t r = floor_sqrt(delta);
        if (r * r != delta) continue;        /* Delta must be a perfect square */

        /* Parity condition:  r and y must have the same parity (since x = (-y ± r)/2). */
        if (((r ^ y) & 1) != 0) continue;

        seq_t cnt = (r == 0) ? 1 : 2;        /* number of x solutions for this y */
        if (y == 0) {
            total += cnt;                    /* y = 0 has no negative counterpart */
        } else {
            total += cnt * 2;                /* count solutions for ±y */
        }
    }

    return total;
}
