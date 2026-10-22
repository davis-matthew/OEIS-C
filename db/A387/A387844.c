/*
 * A387844  a(n) = Sum_{k=0..n} binomial(2*n,5*k).
 * https://oeis.org/A387844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 20*a(n-3) + 15*a(n-4) + 4*a(n-5)
 *            Initial values: a(0)=1, a(1)=1, a(2)=1, a(3)=7, a(4)=57
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)7;
    if (n == 4) return (seq_t)57;

    /* Keep the last five values to compute the next one */
    seq_t t0 = (seq_t)1;   /* a(n-5) */
    seq_t t1 = (seq_t)1;   /* a(n-4) */
    seq_t t2 = (seq_t)1;   /* a(n-3) */
    seq_t t3 = (seq_t)7;   /* a(n-2) */
    seq_t t4 = (seq_t)57;  /* a(n-1) */

    seq_t next;
    for (idx_t i = 5; i <= n; ++i) {
        next = (seq_t)5 * t4
             - (seq_t)10 * t3
             + (seq_t)20 * t2
             + (seq_t)15 * t1
             + (seq_t)4  * t0;

        /* Shift the window for the next iteration */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = t4;
        t4 = next;
    }
    return t4;  /* a(n) */
}
