/*
 * A283352  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 619", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A283352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=10, a(2)=100, a(3)=1110,
 *            a(n)=10*a(n-1)+a(n-2)-10*a(n-3) for n>3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first four terms directly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)10;
    if (n == 2)
        return (seq_t)100;
    if (n == 3)
        return (seq_t)1110;

    /* sliding window for a(n-3), a(n-2), a(n-1) */
    seq_t t0 = (seq_t)10;      /* a(1) */
    seq_t t1 = (seq_t)100;     /* a(2) */
    seq_t t2 = (seq_t)1110;    /* a(3) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        next = t2 * (seq_t)10 + t1 - t0 * (seq_t)10;
        t0 = t1;
        t1 = t2;
        t2 = next;
    }
    return t2;
}
