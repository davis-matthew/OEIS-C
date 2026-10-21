/*
 * A270681  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 190", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=5, a(n)=n^2+4*n for even n>1, a(n)=n^2+6*n-3 for
 *            odd n>1
 * Range:     u8 n <= 14, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    seq_t sn = (seq_t)n;
    if ((n & 1) == 0) {               // even
        return sn * sn + (seq_t)4 * sn;
    } else {                           // odd
        return sn * sn + (seq_t)6 * sn - (seq_t)3;
    }
}
