/*
 * A338544  a(n) = (5*floor((n-1)/2)^2 + (4+(-1)^n)*floor((n-1)/2)) / 2.
 * https://oeis.org/A338544
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*floor((n-1)/2)^2 + (4+(-1)^n)*floor((n-1)/2)) / 2;
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) = a(1) = a(2) = 0 */
    if (n < 3) return 0;

    seq_t k = (seq_t)((n - 1) / 2);  /* floor((n-1)/2) for n >= 3 */

    if ((n & 1) == 0) {              /* n even : 4 + (-1)^n = 5 */
        return (5 * k * k + 5 * k) / 2;
    } else {                         /* n odd  : 4 + (-1)^n = 3 */
        return (5 * k * k + 3 * k) / 2;
    }
}
