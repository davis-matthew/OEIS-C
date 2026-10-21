/*
 * A299102  Number of (n + 1, n + 2)-core partitions with odd parts and
 *          corresponding order ideals confined to the three outermost
 *          diagonals of P_{n + 1, n + 2}.
 * https://oeis.org/A299102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 58, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A299102: Number of (n+1,n+2)-core partitions with odd parts
   confined to the three outermost diagonals of P_{n+1,n+2}.

   The sequence satisfies the linear recurrence
        a(n) = a(n-1) + 7*a(n-2) - 6*a(n-3) - 15*a(n-4)
               + 11*a(n-5) + 12*a(n-6) - 6*a(n-7) - 5*a(n-8)
               + a(n-9) + a(n-10)   for n ≥ 10

   with initial values
        a(0)=1, a(1)=2, a(2)=4, a(3)=7, a(4)=17,
        a(5)=31, a(6)=76, a(7)=144, a(8)=344, a(9)=670.
*/

seq_t a(idx_t n)
{
    static const seq_t init[10] = {1,2,4,7,17,31,76,144,344,670};
    if (n < 10)            /* return known initial terms */
        return init[n];

    seq_t last[10];
    for (idx_t i = 0; i < 10; ++i) last[i] = init[i];

    for (idx_t i = 10; i <= n; ++i)
    {
        /* compute a(i) from the ten previous values */
        seq_t pos = last[9]                     /* a(i-1)         */
                + (seq_t)7 * last[8]            /* 7*a(i-2)       */
                + (seq_t)11 * last[5]           /* 11*a(i-5)      */
                + (seq_t)12 * last[4]           /* 12*a(i-6)      */
                + last[1]                       /* a(i-9)         */
                + last[0];                      /* a(i-10)        */

        seq_t sub = (seq_t)6 * last[7]          /* 6*a(i-3)  */
                + (seq_t)15 * last[6]            /* 15*a(i-4) */
                + (seq_t)6 * last[3]             /* 6*a(i-7)  */
                + (seq_t)5 * last[2];            /* 5*a(i-8)  */

        seq_t next = pos - sub;                 /* a(i)         */

        /* shift the window: drop the oldest term */
        for (idx_t k = 0; k < 9; ++k) last[k] = last[k + 1];
        last[9] = next;
    }

    return last[9];   /* a(n) */
}
