/*
 * A398663  Lower (3/2, 1) midsequence of triangular numbers (A000217) and
 *          tetrahedral numbers (A000330); see Comments.
 * https://oeis.org/A398663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 7*a(n-2) + 8*a(n-3) - 7*a(n-4) + 4*a(n-5) -
 *            a(n-6) with a(0)=0, a(1)=2, a(2)=9, a(3)=23, a(4)=45, a(5)=77
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values */
    const seq_t init[6] = {0, 2, 9, 23, 45, 77};

    if (n < 6) /* use the table directly for the first 6 terms */
        return init[n];

    /* keep the last six computed terms in a rolling buffer */
    seq_t a0 = init[0];
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];
    seq_t a4 = init[4];
    seq_t a5 = init[5];

    for (idx_t i = 6; i <= n; ++i) {
        /* compute a(i) from the recurrence */
        seq_t next = a5 * (seq_t)4
                   - a4 * (seq_t)7
                   + a3 * (seq_t)8
                   - a2 * (seq_t)7
                   + a1 * (seq_t)4
                   - a0;

        /* shift the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }

    return a5; /* a(n) */
}
