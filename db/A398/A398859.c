/*
 * A398859  Upper (4/3, 1)-midsequence of triangular numbers (A000217) and
 *          pentagonal numbers (A000326); see Comments.
 * https://oeis.org/A398859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5), a(0)=0,
 *            a(1)=3, a(2)=9, a(3)=20, a(4)=36
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)20;
    if (n == 4) return (seq_t)36;

    /* Sliding window of the last five values */
    seq_t w0 = (seq_t)0;  /* a(k-5) */
    seq_t w1 = (seq_t)3;  /* a(k-4) */
    seq_t w2 = (seq_t)9;  /* a(k-3) */
    seq_t w3 = (seq_t)20; /* a(k-2) */
    seq_t w4 = (seq_t)36; /* a(k-1) */

    for (idx_t i = 5; i <= n; ++i) {
        /* compute a(k) = 2*w4 - w3 + w2 - 2*w1 + w0 */
        seq_t temp = w4 + w4;       /* 2*w4      */
        temp = temp - w3;           /* - w3      */
        temp = temp + w2;           /* + w2      */
        temp = temp - (w1 + w1);    /* - 2*w1    */
        temp = temp + w0;           /* + w0      */

        /* shift the window for the next iteration */
        w0 = w1;
        w1 = w2;
        w2 = w3;
        w3 = w4;
        w4 = temp;
    }

    return w4;
}
