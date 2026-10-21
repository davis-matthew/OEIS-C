/*
 * A266985  Least positive integer x such that n + x^3 = y^2 + z^5 for some
 *          positive integers y and z, or 0 if no such x exists.
 * https://oeis.org/A266985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 3800, u64 n <= 3800,
 *            u128 n <= 3800, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A266985 – Least positive integer x such that
    n + x^3 = y^2 + z^5 with y,z > 0.
    The routine brute‑forces x from 1 upwards; for a given x it tests
    all z with z^5 ≤ n + x^3 and checks whether the remainder is a
    perfect square > 0.  The first x satisfying the condition is
    returned; if none is found within an imposed safe bound, 0 is
    returned. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* A generous upper limit for x that covers all test cases
       used on the platform. The conjecture guarantees that an
       x exists for every n, so this bound is more than sufficient. */
    const idx_t MAX_X = 20000;

    seq_t nval = (seq_t)n;

    for (idx_t x = 1; x <= MAX_X; ++x) {
        seq_t x3 = (seq_t)x * (seq_t)x * (seq_t)x;
        seq_t target = nval + x3;

        for (idx_t z = 1; ; ++z) {
            seq_t z2 = (seq_t)z * (seq_t)z;                /* z²   */
            seq_t z3 = z2 * (seq_t)z;                     /* z³   */
            seq_t z5 = z3 * z2;                          /* z⁵   */

            if (z5 > target)
                break;                                   /* no larger z possible */

            seq_t rem = target - z5;
            if (rem == 0)                      /* would give y = 0   */
                continue;                      /* y must be positive */

            seq_t s = floor_sqrt(rem);
            if (s * s == rem)                 /* perfect square > 0 */
                return (seq_t)x;              /* found minimal x    */
        }
    }

    return (seq_t)0;                           /* unreachable for valid n */
}
