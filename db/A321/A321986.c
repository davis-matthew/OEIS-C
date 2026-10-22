/*
 * A321986  Number of integer pairs (x,y) with x+y < 3*n/4, x-y < 3*n/4 and
 *          -x/2 < y < 2*x.
 * https://oeis.org/A321986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = number of integer pairs (x,y) satisfying: x + y < 3*n/4,
 *            x - y < 3*n/4, -x/2 < y < 2*x. The implementation iterates over
 *            all possible x and counts admissible y
 * Range:     u8 n <= 30, u16 n <= 482, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 3*n/4 non‑inclusive; use Smax = floor((3*n-1)/4) */
    int64_t N = (int64_t)n;
    int64_t Smax = (3 * N - 1) / 4;               /* floor((3n-1)/4) */

    seq_t count = 0;

    /* For n = 0 or 1, Smax is negative or zero → loop is empty → count = 0. */
    for (int64_t x = 1; x <= Smax; ++x) {
        /* Lower bounds for y */
        int64_t lower1 = x - Smax;                     /* from x + y < 3n/4   */
        int64_t lower2 = - ((x + 1) / 2) + 1;          /* from -x/2 < y         */
        int64_t lower = (lower1 > lower2) ? lower1 : lower2;

        /* Upper bounds for y */
        int64_t upper1 = Smax - x;                     /* from x - y < 3n/4   */
        int64_t upper2 = 2 * x - 1;                    /* from y < 2x          */
        int64_t upper = (upper1 < upper2) ? upper1 : upper2;

        if (lower <= upper) {
            count += (seq_t)(upper - lower + 1);
        }
    }
    return count;
}
