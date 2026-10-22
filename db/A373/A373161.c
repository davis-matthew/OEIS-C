/*
 * A373161  a(n) = numerator of Sum_{k>=0} cos(k*Pi/3)/n^k.
 * https://oeis.org/A373161
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n*(2*n-1)/gcd(6, n-2)
 * Range:     u8 n <= 12, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nt = (seq_t)n;
    seq_t m   = nt - (seq_t)2;
    seq_t G;

    /* gcd(6, m) : 6 = 2*3 */
    if ((m % (seq_t)2) == (seq_t)0)
    {
        if ((m % (seq_t)3) == (seq_t)0)
            G = (seq_t)6;
        else
            G = (seq_t)2;
    }
    else if ((m % (seq_t)3) == (seq_t)0)
        G = (seq_t)3;
    else
        G = (seq_t)1;

    seq_t num = (nt * ((nt * (seq_t)2) - (seq_t)1)) / G;
    return num;
}
