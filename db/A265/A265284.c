/*
 * A265284  Total number of ON (black) cells after n iterations of the "Rule
 *          94" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A265284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(n) = (2*n^2 + 12*n - (-1)^n + 1) / 4 for n > 0
 *            (equivalently: if n even: a(n) = (n^2 + 6*n) / 2 if n odd : a(n)
 *            = (n^2 + 6*n + 1) / 2)
 * Range:     u8 n <= 8, u16 n <= 184, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t nn = (seq_t)n;
    seq_t num;

    if ((n & 1) == 0) /* n even */
    {
        num = (seq_t)2 * nn * nn + (seq_t)12 * nn;
    }
    else /* n odd */
    {
        num = (seq_t)2 * nn * nn + (seq_t)12 * nn + (seq_t)2;
    }

    return num / (seq_t)4;
}
