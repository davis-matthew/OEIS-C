/*
 * A273781  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 929", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=5, a(2)=26; a(n) = (n-1)*(4*n^2+16*n+27)/3 for n>2
 * Range:     u8 n <= 4, u16 n <= 35, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 5;
    if (n == 2) return 26;
    seq_t nn = (seq_t)n;
    seq_t term = 4 * nn * nn + 16 * nn + 27;
    term = (nn - 1) * term / 3;
    return term;
}
