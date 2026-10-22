/*
 * A349957  Number of ways to write n as x^4 + y^2 + (z^2 + 11*16^w)/60, where
 *          x,y,z are nonnegative integers, and w is 0 or 1.
 * https://oeis.org/A349957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non‑negative integer solutions of n = x^4 + y^2
 *            + (z^2 + 11*16^w)/60 with w = 0 or 1
 * Range:     u8 n <= 1, u16 n <= 100, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation notes
 *
 *  * Loop over all admissible x and y, for each compute the
 *    remaining part and check whether a perfect square z exists.
 *  * The two values of w give two constants c = 11 (w=0) and
 *    c = 176 (w=1).  The equation is rearranged as
 *
 *          60*n = 60*x^4 + 60*y^2 + z^2 + c
 *
 *    so z^2 = 60*(n - x^4 - y^2) - c.  If this is a non‑negative
 *    perfect square, we have a valid representation.
 *
 *  * All arithmetic is performed in the generic type seq_t.
 *    We guard against underflow when computing z^2 by checking
 *    that 60*(n - x^4 - y^2)  >=  c before subtracting.
 *
 *  * Complexity: For n ≤ 10⁴, x runs to ⌊n¹/⁴⌋ ≈ 10, y to
 *    ⌊√n⌋ ≈ 100; about 2 × 10³ iterations per call – easily under
 *    the time limit. */

#include <stdbool.h>
#include <stdint.h>
/* floor_sqrt is provided by the utilities. */
#include "../utils/floor_sqrt.h"

/* Function prototypes required by the build system */
extern seq_t floor_sqrt(seq_t x);

/* Count the number of solutions for the given n. */
seq_t a(idx_t n)
{
    seq_t count = 0;
    /* Loop over x */
    for (idx_t x = 0; ; ++x) {
        seq_t xr = (seq_t)x;
        seq_t x4 = xr * xr * xr * xr;
        if (x4 > (seq_t)n) break;

        /* Loop over y */
        for (idx_t y = 0; ; ++y) {
            seq_t yr = (seq_t)y;
            seq_t y2 = yr * yr;
            seq_t sumXY = x4 + y2;
            if (sumXY > (seq_t)n) break;

            /* Remaining part before (z^2 + c)/60 */
            seq_t rem = (seq_t)n - sumXY;
            /* 60*rem may be computed once */
            seq_t remTimes60 = (seq_t)60 * rem;

            /* w = 0  ->  c = 11 */
            if (remTimes60 >= (seq_t)11) {
                seq_t z2 = remTimes60 - (seq_t)11;
                if (z2 >= 0) {
                    seq_t z = floor_sqrt(z2);
                    if (z * z == z2) count++;
                }
            }

            /* w = 1  ->  c = 176 */
            if (remTimes60 >= (seq_t)176) {
                seq_t z2 = remTimes60 - (seq_t)176;
                if (z2 >= 0) {
                    seq_t z = floor_sqrt(z2);
                    if (z * z == z2) count++;
                }
            }
        }
    }

    return count;
}
