/*
 * A273780  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 929", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(1) = 4 a(2) = 21 a(3) = 48 a(n) = (1 + 2*n)^2 for n >
 *            3
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)21;
    if (n == 3) return (seq_t)48;

    seq_t m = (seq_t)n;        /* convert index to seq_t */
    seq_t t = (seq_t)2 * m + (seq_t)1;
    return t * t;
}
