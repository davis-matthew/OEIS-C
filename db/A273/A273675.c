/*
 * A273675  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 833", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=4, a(2)=17, a(n)=n*(2+3*n) for n>=3
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the special initial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)17;

    /* For n >= 3, use the explicit formula */
    seq_t nn = (seq_t)n;
    return nn * ((seq_t)2 + (seq_t)3 * nn);
}
