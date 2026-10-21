/*
 * A273406  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 673", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9 + 3*(-1)^n - 10*n + 48*n^2 + 16*n^3) / 12
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n, n^2, n^3 */
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;

    /* compute 16*n^3 + 48*n^2 */
    seq_t term1 = 16 * n3;
    seq_t term2 = 48 * n2;
    seq_t sum   = term1 + term2;

    /* add the constant part: 12 for even n, 6 for odd n */
    sum += ((n & 1) ? (seq_t)6 : (seq_t)12);

    /* subtract 10*n */
    sum -= 10 * N;

    /* divide by 12 to get the final result */
    return sum / 12;
}
