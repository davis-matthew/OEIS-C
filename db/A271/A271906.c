/*
 * A271906  Size of the largest subset S of the points of an n X n square grid
 *          such that no three of the points of S form a right isosceles
 *          triangle.
 * https://oeis.org/A271906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 3*n - 7 for n >= 6, with the first five values explicitly
 *            given
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the first five terms directly.  The array is 1‑based, so
       index 0 is unused. */
    if (n <= 5) {
        static const seq_t base[6] = {0, 1, 2, 4, 6, 9};
        return base[(size_t)n];
    }
    /* For n >= 6 the closed form a(n) = 3*n - 7 holds. */
    return (seq_t)n * 3 - 7;
}
