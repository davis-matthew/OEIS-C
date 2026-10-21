/*
 * A270934  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 221", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=8, a(2)=4; a(n)=9 for n>2 even; a(n)=4*n^2+4*n-4
 *            for n>2 odd
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;
    if (n == 2) return (seq_t)4;
    if ((n & 1u) == 0)           /* even n > 2 */
        return (seq_t)9;
    else                          /* odd n > 2 */
    {
        seq_t nn = (seq_t)n;
        return (seq_t)4 * (nn * nn + nn - (seq_t)1);
    }
}
