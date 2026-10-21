/*
 * A241372  Number of 3 X n 0..2 arrays with no element equal to fewer
 *          vertical neighbors than horizontal neighbors, with new values 0..2
 *          introduced in row major order.
 * https://oeis.org/A241372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) + 16*a(n-2) - 50*a(n-3) - 72*a(n-4) - 32*a(n-5)
 *            - 32*a(n-6) for n > 8 Initial values: a(1)=5, a(2)=47, a(3)=460,
 *            a(4)=4617, a(5)=46245, a(6)=463567, a(7)=4646421, a(8)=46573979
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1 – unused */
    static const seq_t first[8] = {
        (seq_t)5,        /* a(1) */
        (seq_t)47,       /* a(2) */
        (seq_t)460,      /* a(3) */
        (seq_t)4617,     /* a(4) */
        (seq_t)46245,    /* a(5) */
        (seq_t)463567,   /* a(6) */
        (seq_t)4646421,  /* a(7) */
        (seq_t)46573979  /* a(8) */
    };

    if (n <= 8) return first[n-1];

    /* Sliding‑window variables:
           a1 = a(n-1), a2 = a(n-2), ..., a6 = a(n-6)
       Initial values for n = 9 are a(8), a(7), …, a(3)
    */
    seq_t a1 = first[7];   /* a(8)  */
    seq_t a2 = first[6];   /* a(7)  */
    seq_t a3 = first[5];   /* a(6)  */
    seq_t a4 = first[4];   /* a(5)  */
    seq_t a5 = first[3];   /* a(4)  */
    seq_t a6 = first[2];   /* a(3)  */

    seq_t curr;
    for (idx_t i = 9; i <= n; ++i)
    {
        curr = (seq_t)9  * a1
             + (seq_t)16 * a2
             - (seq_t)50 * a3
             - (seq_t)72 * a4
             - (seq_t)32 * a5
             - (seq_t)32 * a6;

        /* shift window for next step */
        a6 = a5;
        a5 = a4;
        a4 = a3;
        a3 = a2;
        a2 = a1;
        a1 = curr;
    }
    return a1;
}
