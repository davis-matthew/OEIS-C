/*
 * A283590  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 657", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A283590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0, a(2)=101, a(n)=10*a(n-1)+1 for n>=3
 *            (equivalently (10^n-1)/9 for n>=3)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 0;
    if (n == 2) return 101;

    seq_t res = 0;
    /* construct n copies of the digit 1 */
    for (idx_t i = 0; i < n; ++i)
    {
        res = res * 10 + 1;
    }
    return res;
}
