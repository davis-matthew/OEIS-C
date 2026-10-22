/*
 * A385898  a(n) = 16*n^5 + 70*n^4 + 105*n^3 + 65*n^2 + 15*n + 1.
 * https://oeis.org/A385898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n^5 + 70*n^4 + 105*n^3 + 65*n^2 + 15*n + 1
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nseq = (seq_t)n;
    seq_t n2 = nseq * nseq;          // n^2
    seq_t n3 = n2 * nseq;            // n^3
    seq_t n4 = n3 * nseq;            // n^4
    seq_t n5 = n4 * nseq;            // n^5
    seq_t result = 1
        + 16 * n5
        + 70 * n4
        + 105 * n3
        + 65 * n2
        + 15 * nseq;
    return result;
}
