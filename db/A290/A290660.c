/*
 * A290660  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 899", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) + a(n-2) - 10*a(n-3) (n>3) with a(0)=1,
 *            a(1)=11, a(2)=101, a(3)=1111
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial terms, all fit in any seq_t */
    static const seq_t init[4] = { 1, 11, 101, 1111 };

    if (n < 4) return init[n];

    /* use a rolling buffer for the last three terms */
    seq_t a0 = init[0];
    seq_t a1 = init[1];
    seq_t a2 = init[2];
    seq_t a3 = init[3];

    for (idx_t i = 4; i <= n; ++i)
    {
        seq_t next = (10 * a3) + a2 - (10 * a1);
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
