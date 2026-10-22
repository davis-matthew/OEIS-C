/*
 * A360488  31-gonal numbers: a(n) = n*(29*n-27)/2.
 * https://oeis.org/A360488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(29*n-27)/2
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    return nn * (((seq_t)29 * nn) - (seq_t)27) / (seq_t)2;
}
