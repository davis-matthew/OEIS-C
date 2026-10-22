/*
 * A368326  a(0) = 0; a(n) = (1+a(n-1))^4 for n > 0.
 * https://oeis.org/A368326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n=0; a(n) = (1 + a(n-1))^4 for n>0
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t t = res + 1;
        t = t * t;   /* square */
        t = t * t;   /* fourth power */
        res = t;
    }
    return res;
}
