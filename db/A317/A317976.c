/*
 * A317976  a(n) = 2*(a(n-1)+a(n-2)+a(n-3))-a(n-4) for n >= 4, with initial
 *          terms 0,0,1,0.
 * https://oeis.org/A317976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 24, u64 n <= 45, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A317976: a(0)=0, a(1)=0, a(2)=1, a(3)=0;
   recurrence for n≥4: a(n)=2*(a(n-1)+a(n-2)+a(n-3))-a(n-4). */
seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)0;

    /* Use only the last four values to generate the sequence up to n. */
    seq_t a0 = (seq_t)0;   /* a(n-4)  */
    seq_t a1 = (seq_t)0;   /* a(n-3)  */
    seq_t a2 = (seq_t)1;   /* a(n-2)  */
    seq_t a3 = (seq_t)0;   /* a(n-1)  */
    seq_t next = (seq_t)0;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t sum = a3 + a2 + a1;          /* a(n-1)+a(n-2)+a(n-3) */
        next = (sum + sum) - a0;          /* 2*sum - a(n-4)      */
        /* Shift the window for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
