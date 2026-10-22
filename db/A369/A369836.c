/*
 * A369836  Number of compositions of 5*n into parts 1 and 5.
 * https://oeis.org/A369836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5) with
 *            initial values a(0)=1, a(1)=2, a(2)=8, a(3)=34, a(4)=140
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)34;
    if (n == 4) return (seq_t)140;

    /* Sliding window of the five preceding values */
    seq_t a0 = (seq_t)1;   /* a(n-5)   */
    seq_t a1 = (seq_t)2;   /* a(n-4)   */
    seq_t a2 = (seq_t)8;   /* a(n-3)   */
    seq_t a3 = (seq_t)34;  /* a(n-2)   */
    seq_t a4 = (seq_t)140; /* a(n-1)   */

    seq_t next;
    for (idx_t k = 5; k <= n; ++k)
    {
        /* Compute a(k) from the recurrence */
        next = 6 * a4;          /* 6*a(n-1)  */
        next = next - 10 * a3;  /* -10*a(n-2)*/
        next = next + 10 * a2;  /* +10*a(n-3)*/
        next = next - 5 * a1;   /* -5*a(n-4) */
        next = next + a0;       /* +a(n-5)   */

        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4;
}
