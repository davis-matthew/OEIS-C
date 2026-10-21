/*
 * A290491  Number of ways to write 12*n+1 as x^2 + 4*y^2 + 8*z^4, where x and
 *          y are positive integers and z is a nonnegative integer.
 * https://oeis.org/A290491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1284, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the representations of 12*n+1 as x^2 + 4*y^2 + 8*z^4
   with x>0, y>0, z>=0. */
#include "../utils/floor_sqrt.h"
#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t N = (seq_t)12 * n + 1;          /* target value */

    for (idx_t z = 0;; z++) {
        seq_t z4 = (seq_t)z * z * z * z;     /* z^4          */
        seq_t zterm = 8 * z4;                /* 8*z^4        */
        if (zterm > N) break;               /* further z's impossible */

        seq_t rem = N - zterm;              /* remainder for x^2+4*y^2 */

        /* y must satisfy 4*y^2 < rem  (because x^2>0) */
        seq_t max_y2 = rem / 4;
        seq_t y_max = floor_sqrt(max_y2);

        for (idx_t y = 1; y <= y_max; y++) {
            seq_t y2 = (seq_t)y * y;
            seq_t t = rem - 4 * y2;        /* candidate x^2   */
            if (t == 0) continue;         /* x must be >0    */
            seq_t x = floor_sqrt(t);
            if (x * x == t) count++;       /* valid solution   */
        }
    }

    return count;
}
