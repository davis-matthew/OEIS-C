/*
 * A282799  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 505", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A282799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-2) - 4*a(n-4) for n > 3 * initial values: a(0)=1,
 *            a(1)=0, a(2)=7, a(3)=2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)2;

    seq_t a0 = (seq_t)1;            /* a(n-4) */
    seq_t a1 = (seq_t)0;            /* a(n-3) */
    seq_t a2 = (seq_t)7;            /* a(n-2) */
    seq_t a3 = (seq_t)2;            /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t next = (seq_t)5 * a2 - (seq_t)4 * a0;  /* a(n) */
        /* shift values for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;   /* a(n) */
}
