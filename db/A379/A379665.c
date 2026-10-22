/*
 * A379665  Minimum number of intercalates in a Brown's diagonal Latin square
 *          of order 2n.
 * https://oeis.org/A379665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   a(n) = n^2 for n >= 3; a(0)=0, a(1)=0, a(2)=12
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 0;
    if (n == 2) return (seq_t)12;
    seq_t nn = (seq_t)n;
    return nn * nn;
}
