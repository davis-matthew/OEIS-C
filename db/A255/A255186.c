/*
 * A255186  27-gonal numbers: a(n) = n*(25*n-23)/2.
 * https://oeis.org/A255186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(25*n-23)/2
 * Range:     u8 n <= 5, u16 n <= 72, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    return nn * ((nn * 25) - 23) / 2;
}
