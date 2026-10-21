/*
 * A253067  The subsequence A253065(2^n-1).
 * https://oeis.org/A253067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A253067: a subsequence of A253065(2^n-1)
   Linear recurrence with order 5:
   a(0)=1, a(1)=5, a(2)=17, a(3)=65, a(4)=229
   a(n) = a(n-1) + 5*a(n-2) + 13*a(n-3) + 6*a(n-4) + 8*a(n-5)  for n >= 5 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first five terms directly */
    static const seq_t init[5] = { 1, 5, 17, 65, 229 };
    if (n < 5) return init[n];

    /* Iteratively apply the recurrence */
    seq_t a0 = init[0];
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t a4 = init[4];

    for (idx_t i = 5; i <= n; ++i)
    {
        seq_t an = a4
                 + (seq_t)5 * a3
                 + (seq_t)13 * a2
                 + (seq_t)6 * a1
                 + (seq_t)8 * a0;

        /* Shift the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }

    return a4;
}
