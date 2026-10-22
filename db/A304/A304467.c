/*
 * A304467  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2, 3
 *          or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 43, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) +8*a(n-3) -21*a(n-4) -26*a(n-5) +10*a(n-6)
                     +31*a(n-7) +20*a(n-8) -20*a(n-9) -10*a(n-10)   (n>12)
   Initial values: a(1)=4, a(2)=21, a(3)=41, a(4)=117, a(5)=316, a(6)=810,
   a(7)=2208, a(8)=6012, a(9)=15837, a(10)=42579, a(11)=115618,
   a(12)=309144.
*/

seq_t a(idx_t n)
{
    /*            a(1) a(2) a(3) a(4)  a(5)  a(6)  a(7)  a(8)   a(9)    a(10)   a(11)   a(12) */
    static const seq_t init[12] = {4, 21, 41, 117, 316, 810, 2208, 6012, 15837, 42579, 115618, 309144};

    if (n <= 12)
        return init[n - 1];

    /* previous 10 values: a(n-1), a(n-2), ..., a(n-10) */
    seq_t prev[10];
    prev[0] = init[11]; /* a(12) */
    prev[1] = init[10]; /* a(11) */
    prev[2] = init[9];  /* a(10) */
    prev[3] = init[8];  /* a(9)  */
    prev[4] = init[7];  /* a(8)  */
    prev[5] = init[6];  /* a(7)  */
    prev[6] = init[5];  /* a(6)  */
    prev[7] = init[4];  /* a(5)  */
    prev[8] = init[3];  /* a(4)  */
    prev[9] = init[2];  /* a(3)  */

    /* coefficients for a(n-1), a(n-2), ..., a(n-10) */
    static const int coeff[10] = {3, 0, 8, -21, -26, 10, 31, 20, -20, -10};

    for (idx_t k = 13; k <= n; ++k) {
        seq_t pos = 0; /* sum of positive contributions */
        seq_t neg = 0; /* sum of negative contributions */

        for (int i = 0; i < 10; ++i) {
            const seq_t val = prev[i];
            int c = coeff[i];
            if (c > 0)
                pos += val * (seq_t)c;
            else if (c < 0)
                neg += val * (seq_t)(-c);
        }

        seq_t cur = pos - neg;           /* recurrence value */
        if (k == n)
            return cur;                   /* final answer */

        /* shift for next iteration: drop the oldest value */
        for (int i = 9; i > 0; --i)
            prev[i] = prev[i - 1];
        prev[0] = cur;
    }

    /* unreachable for n >= 13 */
    return 0;
}
