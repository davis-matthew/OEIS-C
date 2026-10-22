/*
 * A384600  Expansion of (1+x-x^2) / (1-x-4*x^2+2*x^3+2*x^4).
 * https://oeis.org/A384600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) satisfies a recurrence from the generating function
 *            (1+x-x^2)/(1-x-4*x^2+2*x^3+2*x^4) a(0)=1, a(1)=2, a(2)=5,
 *            a(3)=11 a(n) = a(n-1) + 4*a(n-2) - 2*a(n-3) - 2*a(n-4) for n ≥ 4
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 55, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)11;

    seq_t a0 = (seq_t)1,   /* a(n-4) */
          a1 = (seq_t)2,   /* a(n-3) */
          a2 = (seq_t)5,   /* a(n-2) */
          a3 = (seq_t)11,  /* a(n-1) */
          an;

    for (idx_t i = 4; i <= n; ++i)
    {
        an = a3 + (seq_t)4 * a2 - (seq_t)2 * a1 - (seq_t)2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }

    return a3;
}
