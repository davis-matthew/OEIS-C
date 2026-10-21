/*
 * A269677  Number of length-n 0..7 arrays with no repeated value differing
 *          from the previous repeated value by other than plus or minus one
 *          modulo 7+1.
 * https://oeis.org/A269677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) - 33*a(n-2) - 63*a(n-3) for n ≥ 4 * with
 *            initial values a(1) = 8, a(2) = 64, a(3) = 504
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)8;
    if (n == (idx_t)2) return (seq_t)64;
    if (n == (idx_t)3) return (seq_t)504;

    seq_t a1 = (seq_t)8;     /* a(1)   */
    seq_t a2 = (seq_t)64;    /* a(2)   */
    seq_t a3 = (seq_t)504;   /* a(3)   */
    seq_t an = 0;

    for (idx_t i = (idx_t)4; i <= n; ++i) {
        an = (seq_t)13 * a3 - (seq_t)33 * a2 - (seq_t)63 * a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return an;
}
