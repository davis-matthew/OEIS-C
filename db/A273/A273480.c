/*
 * A273480  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 737", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(n) = (4*n^3 + 12*n^2 + 11*n - 12)/3 for n > 0 (OEIS
 *            A273480)
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    seq_t val = 4 * N3;
    val += 12 * N2;
    val += 11 * N;
    val -= 12;
    val /= 3;
    return val;
}
