/*
 * A270454  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 163", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n even, a(n) = n^2 + 2*n + 1; for n odd, a(n) = 3*n^2 + 2*n
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    if ((n & 1) == 0)   /* n is even */
        return tn * tn + 2 * tn + 1;
    else                /* n is odd */
        return 3 * tn * tn + 2 * tn;
}
