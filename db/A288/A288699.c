/*
 * A288699  Decimal representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 494", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A288699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=0; for n>=3, a(n)=2 if n is even, else
 *            a(n)=1
 * Range:     u8 n <= 126, u16 n <= 126, u32 n <= 126, u64 n <= 126,
 *            u128 n <= 126, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return 1;
    if (n == 2)
        return 0;
    /* n >= 3 */
    return (n & 1) ? 1 : 2;
}
