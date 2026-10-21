/*
 * A273768  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 913", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 + 12*n^2 + 11*n - 39)/3 for n>2, with explicit
 *            initial terms a(0)=1, a(1)=5, a(2)=22
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0ULL) return (seq_t)1;
    if (n == 1ULL) return (seq_t)5;
    if (n == 2ULL) return (seq_t)22;

    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t num = (seq_t)4 * n3 + (seq_t)12 * n2 + (seq_t)11 * N - (seq_t)39;

    return num / (seq_t)3;
}
