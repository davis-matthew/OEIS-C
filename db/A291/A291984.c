/*
 * A291984  Expansion of 1/((1+x)*(1+x^2)*(1+x^3)*(1+x^4)).
 * https://oeis.org/A291984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) =
 *            -(a(n-1)+a(n-2)+2*a(n-3)+2*a(n-4)+2*a(n-5)+2*a(n-6)+2*a(n-7)+a(n-8)+a(n-9)+a(n-10))
 *            Initial values: a(0)=1, a(1)=-1, a(2)=0, a(3)=-1, a(4)=1,
 *            a(5)=0, a(6)=1, a(7)=-1, a(8)=1, a(9)=-2
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial values a(0)..a(9) */
    static const seq_t init[10] = {
        (seq_t)1,   /* a(0) */
       -(seq_t)1,   /* a(1) */
        (seq_t)0,   /* a(2) */
       -(seq_t)1,   /* a(3) */
        (seq_t)1,   /* a(4) */
        (seq_t)0,   /* a(5) */
        (seq_t)1,   /* a(6) */
       -(seq_t)1,   /* a(7) */
        (seq_t)1,   /* a(8) */
       -(seq_t)2    /* a(9) */
    };

    if (n < 10) return init[(size_t)n];

    /* last[0] will hold a(i-1), last[1] a(i-2), ..., last[9] a(i-10) */
    seq_t last[10];
    for (int i = 0; i < 10; ++i)
        last[i] = init[9 - i];   /* reverse init so last[0] = a(9) */

    for (idx_t i = 10; i <= n; ++i)
    {
        seq_t val = -( last[0] + last[1]
                       + (last[2] * 2)
                       + (last[3] * 2)
                       + (last[4] * 2)
                       + (last[5] * 2)
                       + (last[6] * 2)
                       + last[7]
                       + last[8]
                       + last[9] );

        for (int k = 9; k >= 1; --k)
            last[k] = last[k - 1];
        last[0] = val;
    }

    return last[0];
}
