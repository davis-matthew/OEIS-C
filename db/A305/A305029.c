/*
 * A305029  Period 10 sequence [ 0, 2, 2, 2, 2, 0, -2, -2, -2, -2, ...] except
 *          a(0) = 1.
 * https://oeis.org/A305029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1; for n>0, a(n) follows a period‑10 cycle
 *            [2,2,2,2,0,-2,-2,-2,-2,0]
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) is a special case */
    if (n == 0)
        return (seq_t)1;

    /* period‑10 cycle for n>0 */
    static const seq_t cycle[10] = {
        (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)0,
        (seq_t)-2, (seq_t)-2, (seq_t)-2, (seq_t)-2, (seq_t)0
    };

    /* n-1 is safe because we handled n==0 above */
    idx_t r = (n - 1) % 10;
    return cycle[r];
}
