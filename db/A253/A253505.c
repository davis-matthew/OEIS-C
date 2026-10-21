/*
 * A253505  Number of (n+2) X (3+2) 0..1 arrays with every 2 X 2 and 3 X 3
 *          subblock diagonal maximum minus antidiagonal minimum nondecreasing
 *          horizontally and vertically.
 * https://oeis.org/A253505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (10/3)*n^3 + 64*n^2 + (566/3)*n + 92 for n > 4 Initial
 *            terms: a(1)=428, a(2)=784, a(3)=1332, a(4)=2088
 * Range:     u8 none, u16 n <= 13, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1) return 428;
    if (n == 2) return 784;
    if (n == 3) return 1332;
    if (n == 4) return 2088;
    /* n >= 5 */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t num = 10 * n3 + 192 * n2 + 566 * nn + 276;
    return num / 3;
}
