/*
 * A273790  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 931", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(n) = (4*n^3 + 12*n^2 + 11*n - 9)/3 for n>0
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t nn = (seq_t)n;
    seq_t t1 = 4 * nn * nn * nn;
    seq_t t2 = 12 * nn * nn;
    seq_t t3 = 11 * nn;
    seq_t res = (t1 + t2 + t3 - 9) / 3;
    return res;
}
