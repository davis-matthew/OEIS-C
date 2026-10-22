/*
 * A353207  Number of edge cuts in the n-helm graph.
 * https://oeis.org/A353207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) - 46*a(n-2) + 50*a(n-3) - 16*a(n-4), with
 *            initial values a(0)=1, a(1)=6, a(2)=54, a(3)=474
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)54;
    if (n == 3) return (seq_t)474;

    /* Use a sliding window of the last four terms. */
    seq_t a0 = (seq_t)1;   /* a(n-4) */
    seq_t a1 = (seq_t)6;   /* a(n-3) */
    seq_t a2 = (seq_t)54;  /* a(n-2) */
    seq_t a3 = (seq_t)474; /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)13 * a3;      /* 13*a(n-1) */
        next -= (seq_t)46 * a2;           /* -46*a(n-2) */
        next += (seq_t)50 * a1;           /* +50*a(n-3) */
        next -= (seq_t)16 * a0;           /* -16*a(n-4) */

        /* Slide the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
