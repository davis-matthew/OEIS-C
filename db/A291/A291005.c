/*
 * A291005  p-INVERT of (1,1,1,1,1,...), where p(S) = 1 - 2 S - 2 S^3.
 * https://oeis.org/A291005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 7*a(n-2) + 5*a(n-3) for n>=4 Initial values:
 *            a(0)=2, a(1)=6, a(2)=20, a(3)=68
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)20;
    if (n == 3) return (seq_t)68;

    /* a0 = a(n-3), a1 = a(n-2), a2 = a(n-1) */
    seq_t a0 = (seq_t)6;   /* a(1) */
    seq_t a1 = (seq_t)20;  /* a(2) */
    seq_t a2 = (seq_t)68;  /* a(3) */
    seq_t an   = 0;        /* will hold a(n) */

    /* iterate until the desired index */
    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)(5 * a2 - 7 * a1 + 5 * a0);
        a0 = a1;
        a1 = a2;
        a2 = an;
    }

    return an;
}
