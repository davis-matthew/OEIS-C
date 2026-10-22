/*
 * A345008  a(0) = 1; a(4*n) = a(n) - a(n-1), a(4*n+1) = a(4*n+2) = a(4*n+3) =
 *          a(n).
 * https://oeis.org/A345008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; a(4*n) = a(n) - a(n-1); a(4*n+1) = a(4*n+2) = a(4*n+3)
 *            = a(n). This implementation uses simple recursion with an O(log
 *            n) depth
 * Range:     i8 n <= 105, i16 n <= 105, i32 n <= 105, i64 n <= 105,
 *            i128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0) return (seq_t)1;

    /* Decompose n into 4*m + r (0 ≤ r < 4) */
    idx_t m = n / 4;
    int r = n % 4;

    if (r == 0) {
        /* n is a multiple of 4, use the recurrence a(4*m) = a(m) - a(m-1) */
        return a(m) - a(m - 1);
    } else {
        /* n is 4*m+1, 4*m+2, or 4*m+3, all equal to a(m) */
        return a(m);
    }
}
