/*
 * A272007  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 413", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A272007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n odd, a(n)=4*n*(n+1); for n even > 0, a(n)=5
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    /* even index > 0 */
    if ((n & 1) == 0)
        return (seq_t)5;
    /* odd index */
    seq_t nn = (seq_t)n;
    return (seq_t)4 * nn * (nn + (seq_t)1);
}
