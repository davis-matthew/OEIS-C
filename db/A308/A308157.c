/*
 * A308157  Sum of the perimeters of all integer-sided obtuse triangles with
 *          perimeter n.
 * https://oeis.org/A308157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * A070101(n) where A070101(n) counts integer‐sided
 *            obtuse triangles with perimeter n (c is the longest side, c^2 >
 *            a^2 + b^2). The function iterates over all possible longest
 *            sides and all feasible shorter sides, counting the obtuse cases
 *            and multiplying by n
 * Range:     u8 n <= 26, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for completeness – no bools used */

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* outside the defined offset */

    idx_t count = 0;

    /* c is the largest side.  Triangle inequalities give
       ceil(n/3) <= c < n/2  -->  c from (n+2)/3 to (n-1)/2 inclusive */
    idx_t c_start = (n + 2) / 3;          /* ceil(n/3) */
    idx_t c_end   = (n - 1) / 2;          /* floor((n-1)/2) */

    for (idx_t c = c_start; c <= c_end; ++c) {
        /* a ranges such that a <= b <= c and a + b = n - c.
           The conditions simplify to:
              a >= max(1, n - 2c)
              a <= (n - c)/2 */
        idx_t a_start = (n > 2 * c) ? (n - 2 * c) : (idx_t)1;
        idx_t a_end   = (n - c) / 2;

        if (a_start > a_end) continue;

        for (idx_t a = a_start; a <= a_end; ++a) {
            idx_t b = n - c - a;           /* automatically satisfies a <= b <= c */

            if (c * c > a * a + b * b) {   /* obtuse condition */
                ++count;
            }
        }
    }

    return (seq_t)n * (seq_t)count;
}
