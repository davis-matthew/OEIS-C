/*
 * A387627  a(n) = Sum_{k=0..floor(n/2)} 2^k * binomial(2*n-2*k+1,2*k+1).
 * https://oeis.org/A387627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) + 4*a(n-3) - 4*a(n-4) Initial values:
 *            a(0)=1, a(1)=3, a(2)=7, a(3)=27 Recurrence holds for all n ≥ 4
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)27;

    /* Variables hold a(n-4), a(n-3), a(n-2), a(n-1) respectively. */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)3;   /* a(1) */
    seq_t a2 = (seq_t)7;   /* a(2) */
    seq_t a3 = (seq_t)27;  /* a(3) */
    seq_t an = 0;

    for (idx_t i = 4; i <= n; ++i) {
        an = a3 * (seq_t)2
           + a2 * (seq_t)3
           + a1 * (seq_t)4
           - a0 * (seq_t)4;
        /* Slide the window for the next iteration. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
