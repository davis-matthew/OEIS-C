/*
 * A389798  Centered 29-gonal numbers.
 * https://oeis.org/A389798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (29*n^2 - 29*n + 2)/2
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n is 1-based index */
    seq_t nn = (seq_t)n;
    seq_t term = ((seq_t)29 * nn * nn - (seq_t)29 * nn + 2) / 2;
    return term;
}
