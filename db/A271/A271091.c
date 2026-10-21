/*
 * A271091  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 275", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A271091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 5; for n > 1 * a(n) = 5 if n is even, * a(n) =
 *            4*n^2 + 4*n - 8 if n is odd
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if ((n & 1ULL) == 0)            /* even n > 1 */
        return (seq_t)5;
    seq_t nn = (seq_t)n;
    return 4 * nn * nn + 4 * nn - (seq_t)8;
}
