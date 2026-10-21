/*
 * A273677  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 833", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3+5*n^2+3*n+2)/2 for n>1, a(0)=1, a(1)=5
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t N = (seq_t)n;
    seq_t term1 = 2 * N * N * N;   /* 2*n^3 */
    seq_t term2 = 5 * N * N;       /* 5*n^2 */
    seq_t term3 = 3 * N;           /* 3*n */
    seq_t sum   = term1 + term2 + term3 + 2;
    return sum / 2;
}
