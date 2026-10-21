/*
 * A299641  Solution (d(n)) of the system of 5 complementary equations in
 *          Comments.
 * https://oeis.org/A299641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-4) - a(n-5) (n>4) Initial values: a(0)=4,
 *            a(1)=8, a(2)=13, a(3)=17, a(4)=21
 * Range:     u8 n <= 59, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial known terms */
    static const seq_t init[5] = {4, 8, 13, 17, 21};

    if (n < 5)
        return init[n];

    /* keep the last five values needed for the recurrence:
       a(i-5), a(i-4), a(i-3), a(i-2), a(i-1)   */
    seq_t a0 = init[0];   /* a(i-5)  */
    seq_t a1 = init[1];   /* a(i-4)  */
    seq_t a2 = init[2];   /* a(i-3)  */
    seq_t a3 = init[3];   /* a(i-2)  */
    seq_t a4 = init[4];   /* a(i-1)  */
    seq_t an = a4;        /* placeholder for a(i) */

    for (idx_t i = 5; i <= n; ++i) {
        an = a4 + a1 - a0;   /* a(i) = a(i-1) + a(i-4) - a(i-5) */
        /* shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }

    return an;
}
