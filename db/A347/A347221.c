/*
 * A347221  Square array T(s,t), s >= 0, t >= 0, read by downward
 *          antidiagonals: T(s,t) is the number of nonnegative ordered triples
 *          (a,b,c) satisfying a+b+c <= s and a*b*c <= t.
 * https://oeis.org/A347221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = number of nonnegative integer triples (a,b,c) with a+b+c
 *            <= s and a*b*c <= t, where (s,t) are the antidiagonal
 *            coordinates of index n: 0,1,2,3,... read by downward
 *            antidiagonals in the square array T(s,t)
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   antidiagonal_coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/antidiagonal_coords.h"

seq_t a(idx_t n) {
    /* Convert index to array coordinates (s,t). */
    idx_t s, t;
    antidiagonal_coords(n, &s, &t);

    seq_t total = 0;

    for (idx_t a = 0; a <= s; ++a) {
        idx_t rem_ab = s - a;            /* remaining sum for b and c */
        for (idx_t b = 0; b <= rem_ab; ++b) {
            seq_t ab = (seq_t)a * b;     /* product a*b */
            idx_t rem_c = rem_ab - b;    /* max allowed c to satisfy a+b+c <= s */

            if (ab == 0) {
                /* a*b = 0 => product condition always satisfied */
                total += (seq_t)(rem_c + 1);
            } else {
                seq_t cmax = t / ab;      /* max c from product constraint */
                if (cmax > (seq_t)rem_c) cmax = rem_c;
                total += cmax + 1;        /* c ranges from 0 to cmax inclusive */
            }
        }
    }

    return total;
}
