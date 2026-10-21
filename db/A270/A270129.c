/*
 * A270129  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 89", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(0) = 1 a(1) = 4 a(2) = 5 a(3) = 44 for n > 3: a(n) = 0
 *            if n is even; a(n) = 4*n^2 + 4*n + 1 if n is odd
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first four values */
    if (n == 0) return 1;
    if (n == 1) return 4;
    if (n == 2) return 5;
    if (n == 3) return 44;

    /* for n >= 4 */
    if ((n & 1) == 0)          /* even n */
        return 0;
    else                       /* odd n */
    {
        seq_t nn = (seq_t)n;
        /* compute 4*nn*nn + 4*nn + 1 */
        seq_t term = (seq_t)4 * nn * nn;
        term += (seq_t)4 * nn;
        term += 1;
        return term;
    }
}
