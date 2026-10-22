/*
 * A338241  For any m >= 0, a(3*m) = 3*a(m), a(3*m+1) = 1-3*a(m), a(3*m+2) =
 *          3*a(m)-1.
 * https://oeis.org/A338241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(3*m) = 3*a(m), a(3*m+1) = 1-3*a(m), a(3*m+2) = 3*a(m)-1; a(0)
 *            = 0
 * Range:     i8 n <= 242, i16 n <= 6560, i32 n <= 6560, i64 n <= 6560,
 *            i128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    seq_t sub = a(n / 3);
    idx_t r = n % 3;
    if (r == 0) {
        return sub * 3;
    } else if (r == 1) {
        return 1 - sub * 3;
    } else { /* r == 2 */
        return sub * 3 - 1;
    }
}
