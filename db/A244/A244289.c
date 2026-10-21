/*
 * A244289  Numbers n such that floor( n^(3/2) ) is a concatenation of two
 *          successive numbers.
 * https://oeis.org/A244289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   concat_parts, floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = the nth integer such that floor(n^(3/2)) is a concatenation of two successive integers. */
#include <stdbool.h>
#include "../utils/concat_parts.h"
#include "../utils/floor_cube_root.h"

seq_t a(idx_t target)
{
    if (target == 0)
        return 0;

    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        /* build the concatenated number m = k followed by k+1 */
        seq_t m     = concat_parts(k, k + 1);
        seq_t m2    = m * m;

        /* find the largest integer n with n^3 <= m^2 */
        seq_t n = floor_cube_root(m2);
        if (n * n * n < m2) ++n;   /* adjust if it was too small */

        /* check the strict upper bound (m+1)^2 */
        seq_t m1   = m + 1;
        seq_t m12  = m1 * m1;
        if (n * n * n < m12) {
            ++count;
            if (count == target)
                return n;
        }
    }
}
