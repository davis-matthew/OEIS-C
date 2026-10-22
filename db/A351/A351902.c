/*
 * A351902  Number of ways to write n as w^2 + x^2 + y^2 + z^2 + 3*x*y*z,
 *          where w is a positive integer, and x,y,z are nonnegative integers
 *          with x <= y <= z.
 * https://oeis.org/A351902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # { (w,x,y,z) | w>0, 0<=x<=y<=z, * w^2 + x^2 + y^2 + z^2
 *            + 3*x*y*z = n }
 * Range:     u8 n <= 36, u16 n <= 3827, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t nseq = (seq_t)n;
    /* x ranges from 0 up to sqrt(n) approximately. */
    for (idx_t x = 0;; ++x) {
        seq_t x2 = (seq_t)x * (seq_t)x;
        if (x2 > nseq - 1) break;        /* minimal sum already exceeds n-1   */
        for (idx_t y = x;; ++y) {
            seq_t y2 = (seq_t)y * (seq_t)y;
            seq_t sum_xy = x2 + y2;
            if (sum_xy > nseq - 1) break; /* no larger y will fit           */
            for (idx_t z = y;; ++z) {
                seq_t z2 = (seq_t)z * (seq_t)z;
                seq_t term = sum_xy + z2 +
                    ((seq_t)3 * (seq_t)x * (seq_t)y * (seq_t)z);
                if (term > nseq - 1) break;  /* z too large for this x,y      */
                seq_t t = nseq - term;       /* w^2 must be this positive value */
                if (t == 0) continue;        /* w must be positive              */
                seq_t w = floor_sqrt(t);
                if (w * w == t) ++count;     /* valid representation             */
            }
        }
    }
    return count;
}
