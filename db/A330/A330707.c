/*
 * A330707  a(n) = ( 3*n^2 + n - 1 + (-1)^floor(n/2) )/4.
 * https://oeis.org/A330707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^2 + n - 1 + (-1)^floor(n/2))/4
 * Range:     u8 n <= 9, u16 n <= 170, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t sn = n;
    /* compute 3*sn*sn + sn - 1   (all unsigned so safe for n>=1) */
    seq_t num = sn * sn;
    num = num * 3 + sn - 1;

    /* (-1)^floor(n/2) : 1 if floor(n/2) even, -1 otherwise */
    bool sign_neg = ((n / 2) & 1U);   /* floor(n/2) odd → subtract 1 */
    if (sign_neg)
        num -= 1;
    else
        num += 1;

    return num / 4;
}
