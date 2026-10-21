/*
 * A273405  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 673", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>0, a(n)=4*n*n+4*n-3 if n even, else
 *            a(n)=4*(n*n+n-1)
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t nn = (seq_t)n;
    if ((n & (idx_t)1) == 0)          /* n even */
    {
        return (seq_t)4 * nn * nn + (seq_t)4 * nn - (seq_t)3;
    }
    else                               /* n odd */
    {
        return (seq_t)4 * (nn * nn + nn - (seq_t)1);
    }
}
