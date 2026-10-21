/*
 * A270006  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 5", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(1)=8; a(2)=0; a(n)=0 for even n>1; a(n)=(2*n+1)^2 for
 *            odd n>1
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 8;
    if (n == 2) return 0;
    if ((n & 1) == 0) return 0;
    seq_t x = (seq_t)n;
    x = x * 2 + 1;
    return x * x;
}
