/*
 * A273744  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 901", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = n * (11 + 12*n + 4*n^2) / 3 for n > 0
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t nn = (seq_t)n;
    seq_t temp = (seq_t)11 + (seq_t)12 * nn + (seq_t)4 * nn * nn;
    seq_t num = nn * temp;
    return num / (seq_t)3;
}
