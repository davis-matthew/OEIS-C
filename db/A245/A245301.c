/*
 * A245301  a(n) = n*(7*n^2 + 15*n + 8)/6.
 * https://oeis.org/A245301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(7*n+8)/6
 * Range:     u8 n <= 5, u16 n <= 37, u32 n <= 849, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t1 = nn * (nn + (seq_t)1);
    seq_t t2 = ((seq_t)7 * nn) + (seq_t)8;
    return t1 * t2 / (seq_t)6;
}
