/*
 * A398661  Lower (1/2, 1) midsequence of triangular numbers (A000217) and
 *          tetrahedral numbers (A000330); see Comments.
 * https://oeis.org/A398661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 7*a(n-2) + 8*a(n-3) - 7*a(n-4) + 4*a(n-5) -
 *            a(n-6); initial values: a(0)=0, a(1)=1, a(2)=6, a(3)=17,
 *            a(4)=35, a(5)=62
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first six terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;
    if (n == 3) return (seq_t)17;
    if (n == 4) return (seq_t)35;
    if (n == 5) return (seq_t)62;

    /* Recurrence requires the six preceding values.  We keep them in
       a small sliding window and update it in each iteration. */
    seq_t a0 = (seq_t)0;   /* a(n-6) */
    seq_t a1 = (seq_t)1;   /* a(n-5) */
    seq_t a2 = (seq_t)6;   /* a(n-4) */
    seq_t a3 = (seq_t)17;  /* a(n-3) */
    seq_t a4 = (seq_t)35;  /* a(n-2) */
    seq_t a5 = (seq_t)62;  /* a(n-1) */

    idx_t i;
    for (i = 6; i <= n; ++i) {
        seq_t next = 4 * a5 - 7 * a4 + 8 * a3 - 7 * a2 + 4 * a1 - a0;
        /* Slide the window forward */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    return a5;
}
