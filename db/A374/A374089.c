/*
 * A374089  Number of ordered pairs (x,y) of positive integers such that x^2 +
 *          3*x*y + y^2 = n.
 * https://oeis.org/A374089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts ordered pairs (x,y) of positive integers with * x^2
 *            + 3*x*y + y^2 = n. * * Computation: iterate x from 1 to ⌊√n⌋.
 *            For each x compute the * discriminant D = 5*x^2 + 4*n. If D is a
 *            perfect square s, then * y = (s - 3*x)/2 must be a positive
 *            integer. Each such solution * contributes one...
 * Range:     u8 n <= 8, u16 n <= 1246, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* If n is 0 or 1, the loop will naturally return 0.  */
    seq_t count = 0;

    /* Compute floor(sqrt(n)) once as the upper bound for x. */
    seq_t n_seq = (seq_t)n;
    seq_t x_max = floor_sqrt(n_seq);
    for (idx_t x_ = 1; x_ <= x_max; ++x_) {
        seq_t x = (seq_t)x_;
        /* D = 5*x^2 + 4*n   (all in seq_t to avoid overflow). */
        seq_t D = 5 * x * x + 4 * n_seq;
        seq_t s = floor_sqrt(D);
        if (s * s != D) continue;        /* D is not a perfect square. */

        /* Need s > 3*x for y > 0 and (s - 3*x) even. */
        if (s <= 3 * x) continue;
        if (((s - 3 * x) & 1) != 0) continue;   /* parity check */

        /* y = (s - 3*x)/2 is positive integer at this point. */
        /* Count this ordered pair (x,y).   */
        ++count;
    }
    return count;
}
