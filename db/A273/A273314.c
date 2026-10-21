/*
 * A273314  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 643", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 + 12*n^2 - 13*n + 15) / 3 for n > 0, a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;          /* a(0) = 1 */

    seq_t nn = (seq_t)n;
    seq_t n_sq = nn * nn;          /* n^2 */
    seq_t n_cube = n_sq * nn;      /* n^3 */

    seq_t term1 = ((seq_t)4) * n_cube;
    seq_t term2 = ((seq_t)12) * n_sq;
    seq_t term3 = ((seq_t)13) * nn;

    seq_t sum = term1 + term2 - term3 + ((seq_t)15);
    return sum / ((seq_t)3);
}
