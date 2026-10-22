/*
 * A357259  a(n) is the number of 2 X 2 Euclid-reduced matrices having
 *          determinant n.
 * https://oeis.org/A357259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} max(d - n/d, 1)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;

    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t d2 = n / d;
            if (d == d2) {
                sum += 1;                       /* d == n/d  →  max(0,1) = 1 */
            } else {
                /* d < d2  →  d2 - d is positive,
                 * max(d2 - d, 1) = d2 - d.  Add the divisor d itself: +1 */
                sum += (seq_t)(d2 - d + 1);
            }
        }
    }
    return sum;
}
