/*
 * A397973  Upper (1,1/3) midsequence of midsequence of triangular numbers
 *          (A000217) and squares (A000290); see Comments.
 * https://oeis.org/A397973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5), with
 *            initial values a(0)=0, a(1)=2, a(2)=5, a(3)=9, a(4)=16
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms explicitly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)9;
    if (n == 4) return (seq_t)16;

    /* Keep the last five terms: v0 = a(n-5), v1 = a(n-4), ..., v4 = a(n-1) */
    seq_t v0 = (seq_t)0;   /* a(0)  */
    seq_t v1 = (seq_t)2;   /* a(1)  */
    seq_t v2 = (seq_t)5;   /* a(2)  */
    seq_t v3 = (seq_t)9;   /* a(3)  */
    seq_t v4 = (seq_t)16;  /* a(4)  */

    idx_t i;
    for (i = 5; i <= n; ++i) {
        /* Compute a(i) from the recurrence */
        seq_t next = (seq_t)2 * v4 - v3 + v2 - (seq_t)2 * v1 + v0;
        /* Shift the window */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = v4;
        v4 = next;
    }
    return v4;
}
