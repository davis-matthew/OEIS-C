/*
 * A288961  Number of 3-cycles in the n X n rook graph.
 * https://oeis.org/A288961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*binomial(n,3)
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 337, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 2) {
        return (seq_t)0;
    }
    seq_t nn = (seq_t)n;
    seq_t comb = nn * (nn - 1) * (nn - 2) / 6;
    return (seq_t)2 * nn * comb;
}
