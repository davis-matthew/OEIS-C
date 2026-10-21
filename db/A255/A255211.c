/*
 * A255211  a(n) = n*(n+1)*(7*n+2)/6.
 * https://oeis.org/A255211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(7*n+2)/6
 * Range:     u8 n <= 5, u16 n <= 37, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t A = (seq_t)n;
    seq_t B = A + (seq_t)1;
    seq_t C = (seq_t)7 * A + (seq_t)2;

    /* divide by 2 */
    if ((A & (seq_t)1) == 0)
        A /= 2;
    else if ((B & (seq_t)1) == 0)
        B /= 2;
    else
        C /= 2;

    /* divide by 3 */
    if (A % (seq_t)3 == 0)
        A /= 3;
    else if (B % (seq_t)3 == 0)
        B /= 3;
    else
        C /= 3;

    return A * B * C;
}
