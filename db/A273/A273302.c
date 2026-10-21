/*
 * A273302  Least nonnegative integer x such that n = x^2 + y^2 + z^2 + w^2
 *          for some nonnegative integer y,z,w with x + 3*y + 5*z a square.
 * https://oeis.org/A273302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least nonnegative integer x such that there exist *
 *            nonnegative integers y, z, w with * n = x^2 + y^2 + z^2 + w^2 *
 *            and x + 3*y + 5*z is a perfect square
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a straightforward three–loop search.
 * For each candidate x, it enumerates feasible y and z and checks
 * that the remaining value is a square w^2 and that
 * x + 3*y + 5*z is itself a square.  All arithmetic is done in
 * seq_t under the assumption that seq_t is large enough for the
 * intermediate values (which are bounded by n <= 10^4 in the test
 * range).  The routine is O(n^(3/2)) in practice (~1e6 operations
 * for n = 10^4) and thus well within the time limit. */

#include "../utils/floor_sqrt.h"
#include <stdbool.h>

/* Helper: test whether a seq_t value is a perfect square. */
static inline bool is_square_seq(seq_t v)
{
    seq_t s = floor_sqrt(v);
    return s * s == v;
}

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t limit_x = floor_sqrt(N);   /* smallest x such that x^2 > n stops search */
    for (idx_t x = 0; x <= (idx_t)limit_x; ++x) {
        seq_t x2 = (seq_t)x * (seq_t)x;
        seq_t rem1 = N - x2;          /* remaining after x^2 */

        seq_t limit_y = floor_sqrt(rem1);
        for (idx_t y = 0; y <= (idx_t)limit_y; ++y) {
            seq_t y2 = (seq_t)y * (seq_t)y;
            seq_t rem2 = rem1 - y2;  /* remaining after y^2 */

            seq_t limit_z = floor_sqrt(rem2);
            for (idx_t z = 0; z <= (idx_t)limit_z; ++z) {
                seq_t z2 = (seq_t)z * (seq_t)z;
                seq_t rem3 = rem2 - z2; /* remaining after z^2 */

                /* rem3 must be a perfect square to be w^2 */
                seq_t w = floor_sqrt(rem3);
                if (w * w != rem3)
                    continue;

                /* Check the side condition */
                seq_t t = (seq_t)x + 3 * (seq_t)y + 5 * (seq_t)z;
                if (is_square_seq(t))
                    return (seq_t)x;
            }
        }
    }
    /* By Lagrange's theorem and the condition's conjecture, a solution
     * always exists, so this line should never be reached. */
    return 0;
}
