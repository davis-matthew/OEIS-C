/*
 * A380891  If n mod 2 = 0 then a(n) = floor(n^(1/3)) else a(n) =
 *          floor(n^(4/3)).
 * https://oeis.org/A380891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = floor(n^(1/3))                if n is even
 *            a(n) = floor((n^4)^(1/3)) = floor(n^(4/3))    if n is odd
 *
 * The function uses the supplied utility floor_cube_root(seq_t)
 * to perform the integer cube root calculation on a seq_t value.
 * For odd n we evaluate floor_cube_root of n^4, which equals
 * floor(n^(4/3)).  For all builds the arithmetic is performed
 * in seq_t to avoid overflow for the supported sizes.
 */
#include <stdint.h>
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    seq_t n_s = (seq_t)n;
    if ((n & 1) == 0) {
        /* n is even: a(n) = floor(cuberoot(n)) */
        return floor_cube_root(n_s);
    } else {
        /* n is odd: a(n) = floor(cuberoot(n^4)) = floor(n^(4/3)) */
        seq_t n2 = n_s * n_s;     /* n^2 */
        seq_t n4 = n2 * n2;       /* n^4 */
        return floor_cube_root(n4);
    }
}
