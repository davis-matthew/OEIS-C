/*
 * A374621  Expansion of e.g.f. 1 - x^4/24 - log(1 - x).
 * https://oeis.org/A374621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(4)=5, and for all other n>0: a(n)=(n-1)! (i.e.
 *            a(n)=(n-1)*a(n-1) for n>5)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 4)
        return (seq_t)5;

    seq_t res = (seq_t)1;
    for (idx_t i = 2; i <= n - 1; ++i)
        res *= (seq_t)i;
    return res;
}
