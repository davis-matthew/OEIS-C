/*
 * A250852  Number of (n+1) X (7+1) 0..3 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 35*a(n-2) + 50*a(n-3) - 24*a(n-4) Initial
 *            values: a(1)=27225, a(2)=198297, a(3)=1195457, a(4)=6279401
 * Range:     u8 none, u16 n <= 1, u32 n <= 8, u64 n <= 24, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n should be >= 1; if n == 0, return 0 as a safe fallback */
    if (n == 0)
        return (seq_t)0;

    /* Base cases */
    if (n == 1) return (seq_t)27225;
    if (n == 2) return (seq_t)198297;
    if (n == 3) return (seq_t)1195457;
    if (n == 4) return (seq_t)6279401;

    /* Use the recurrence, keeping only the last four values */
    seq_t a1 = (seq_t)27225;    /* a(n-4) */
    seq_t a2 = (seq_t)198297;   /* a(n-3) */
    seq_t a3 = (seq_t)1195457;  /* a(n-2) */
    seq_t a4 = (seq_t)6279401;  /* a(n-1) */
    seq_t an = a4;              /* placeholder for a(n) */

    for (idx_t i = 5; i <= n; ++i)
    {
        an = (seq_t)10 * a4 - (seq_t)35 * a3 + (seq_t)50 * a2 - (seq_t)24 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }

    return an;
}
